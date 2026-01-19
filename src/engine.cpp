#include "engine.hpp"

#include <spdlog/spdlog.h>
#include <raylib.h>

#include <defer.hpp>
#include <engine/window.hpp>
#include <utility>

namespace muen {

extern "C" auto module_loader(JSContext *ctx, const char *module_name, void *opaque) noexcept -> JSModuleDef *;
auto read_config(JSContext *js, JSValue object) -> Result<GameConfig>;

auto Engine::JSRuntime_deleter::operator()(JSRuntime *rt) noexcept -> void {
    if (rt == nullptr) return;
    JS_FreeRuntime(rt);
}

auto Engine::JSContext_deleter::operator()(JSContext *ctx) noexcept -> void {
    if (ctx == nullptr) return;
    JS_FreeContext(ctx);
}

auto Engine::create(const std::filesystem::path& base_path) noexcept -> Result<std::unique_ptr<Engine>> {
    window::setup();

    SPDLOG_TRACE("Creating JS runtime");
    auto runtime_ptr = JS_NewRuntime();
    if (runtime_ptr == nullptr) return Err(error::create("Could not allocate runtime"));
    auto runtime = std::unique_ptr<JSRuntime, JSRuntime_deleter>(runtime_ptr);

    SPDLOG_TRACE("Creating JS context");
    auto context_ptr = JS_NewContext(runtime.get());
    if (context_ptr == nullptr) return Err(error::create("Could not allocate context"));
    auto context = std::unique_ptr<JSContext, JSContext_deleter>(context_ptr);

    SPDLOG_TRACE("Creating file store");
    auto store = std::unique_ptr<IFileStore> {};
    if (std::filesystem::is_directory(base_path)) {
        if (auto r = FilesystemStore::open(base_path)) store = std::make_unique<FilesystemStore>(std::move(*r));
        else return Err(r.error());
    } else {
        if (auto r = ZipStore::open(base_path)) store = std::make_unique<ZipStore>(std::move(*r));
        else return Err(r.error());
    }

    SPDLOG_TRACE("Allocationg engine");
    auto engine_ptr = owner<Engine *>(new (std::nothrow) Engine {
        std::move(runtime),
        std::move(context),
        std::move(store),
    });
    if (engine_ptr == nullptr) return Err(error::create("Could not allocate engine"));
    auto engine = std::unique_ptr<Engine>(engine_ptr);

    JS_SetDumpFlags(engine->js_runtime(), JS_DUMP_LEAKS);
    JS_SetRuntimeOpaque(engine->js_runtime(), engine.get());
    JS_SetModuleLoaderFunc(engine->js_runtime(), nullptr, module_loader, engine.get());

    SPDLOG_TRACE("Engine created successfully");
    return engine;
}

auto Engine::get(not_null<JSRuntime *> rt) noexcept -> Engine& {
    return *static_cast<Engine *>(JS_GetRuntimeOpaque(rt.get()));
}

auto Engine::get(not_null<JSContext *> ctx) noexcept -> Engine& {
    return *static_cast<Engine *>(JS_GetRuntimeOpaque(JS_GetRuntime(ctx.get())));
}

auto Engine::js_runtime() const noexcept -> not_null<JSRuntime *> {
    return _js_runtime.get().get();
}

auto Engine::js_context() const noexcept -> not_null<JSContext *> {
    return _js_context.get().get();
}

auto Engine::store() const noexcept -> IFileStore& {
    return *_store;
}

auto Engine::register_plugin(const plugins::EnginePlugin& desc) noexcept -> void try {
    for (const auto& [name, module] : desc.c_modules) {
        // TODO: check if already exists
        _c_modules.insert({name, module});
    }

    for (const auto& [name, module] : desc.js_modules) {
        // TODO: check if already exists
        _js_modules.insert({name, module});
    }

    if (desc.load != nullptr) {
        _load_callbacks.emplace_back(desc.load);
    }

    if (desc.unload != nullptr) {
        _unload_callbacks.emplace_back(desc.unload);
    }

    if (desc.update != nullptr) {
        _update_callbacks.emplace_back(desc.update);
    }

    if (desc.draw != nullptr) {
        _draw_callbacks.emplace_back(desc.draw);
    }

    SPDLOG_INFO("Registered `{}` plugin", desc.name);
} catch (...) {
    std::terminate();
}

auto Engine::load_plugins() noexcept -> Result<> try {
    SPDLOG_DEBUG("Loading plugins");
    for (const auto& callback : _load_callbacks) {
        if (auto result = callback(); !result) return Err(std::move(result.error()));
    }
    return {};
} catch (std::exception& e) {
    return Err(error::create(e.what()));
}

[[nodiscard]] auto Engine::run_game(Game& game) noexcept -> Result<> try {
    defer({
        SPDLOG_TRACE("Unloading plugins");
        for (const auto& callback : _unload_callbacks) {
            if (auto result = callback(); !result) {
                SPDLOG_WARN("Error unloading plugin: {}", result.error()->msg());
            }
        }
    });

    SPDLOG_DEBUG("Creating window");
    auto w = window::create(
        window::Config {
            .width = game.config().width,
            .height = game.config().height,
            .fps = game.config().fps,
            .title = game.config().title,
        }
    );
    defer({
        SPDLOG_TRACE("Closing window");
        window::close(w);
    });

    SPDLOG_DEBUG("Loading game");
    if (auto r = game.load(); !r) return Err(r.error());

    SPDLOG_DEBUG("Running rame");
    while (!window::should_close(w)) {
        if (IsKeyPressed(KEY_F5)) {
            if (auto r = game.try_reload(); !r) {
                SPDLOG_ERROR("Exception occured while reloading the game: {}", r.error()->msg());
            }
        }

        SPDLOG_TRACE("Updating plugins");
        for (const auto& callback : _update_callbacks) {
            if (auto r = callback(); !r) return Err(r.error());
        }

        SPDLOG_TRACE("Updating game");
        if (auto r = game.update(); !r) return Err(r.error());

        window::begin_drawing(w);

        SPDLOG_TRACE("Drawing plugins");
        for (const auto& callback : _draw_callbacks) {
            if (auto r = callback(); !r) return Err(r.error());
        }

        SPDLOG_TRACE("Drawing game");
        if (auto r = game.draw(); !r) return Err(r.error());

        window::draw_fps(w);
        window::end_drawing(w);
    }

    return {};
} catch (std::exception& e) {
    return Err(error::create(e.what()));
}

auto Engine::load_module(const std::filesystem::path& name) noexcept -> Result<owner<JSModuleDef *>> try {
    if (auto cm = _c_modules.find(name); cm != _c_modules.end()) {
        SPDLOG_DEBUG("Module {} resolved as builtin native module", name.string());
        return cm->second;
    } else {
        auto code = std::string {};
        if (auto jm = _js_modules.find(name); jm != _js_modules.end()) {
            SPDLOG_DEBUG("Module {} resolved as builtin js module", name.string());
            code = jm->second;
        } else {
            auto path = name;
            // TODO: error handling
            if (!path.has_extension()) {
                path += ".js";
            }
            SPDLOG_TRACE("Loading module {}", path.string());
            const auto contents = _store->read_string(path);
            if (!contents) return Err(contents.error());
            SPDLOG_DEBUG("Module {} resolved as game module", name.string());
            code = *contents;
        }

        JSValue ret = JS_Eval(
            js_context(),
            code.c_str(),
            code.size(),
            name.string().c_str(),
            JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY
        );

        if (JS_IsException(ret)) return nullptr;
        auto mod = static_cast<JSModuleDef *>(JS_VALUE_GET_PTR(ret));
        JS_FreeValue(js_context(), ret);

        return mod;
    }
} catch (std::exception& e) {
    return Err(error::create(e.what()));
}

Engine::Engine(
    std::unique_ptr<JSRuntime, JSRuntime_deleter>&& runtime,
    std::unique_ptr<JSContext, JSContext_deleter>&& context,
    std::unique_ptr<IFileStore>&& store
) noexcept :
    _js_runtime {std::move(runtime)},
    _js_context {std::move(context)},
    _store {std::move(store)} {}

auto Game::create(not_null<JSContext *> js, not_null<IFileStore *> store) -> Result<Game> {
    SPDLOG_TRACE("Creating game");

    SPDLOG_TRACE("Reading game file");
    const auto src = store->read_string("game.js");
    if (!src) return Err(src.error());

    SPDLOG_TRACE("Compiling game module");
    auto mod = JS_Eval(
        js,
        src->c_str(),
        src->size(),
        "game.js",
        JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_STRICT | JS_EVAL_FLAG_COMPILE_ONLY
    );
    if (JS_HasException(js)) return Err(error::from_js(js, JS_GetException(js)));

    SPDLOG_TRACE("Evaluating game module");
    auto eval_ret = JS_EvalFunction(js, mod);
    defer(JS_FreeValue(js, eval_ret));
    auto eval_result = JS_PromiseResult(js, eval_ret);
    if (JS_IsError(eval_result)) return Err(error::from_js(js, eval_result));
    JS_FreeValue(js, eval_result);

    auto m = static_cast<JSModuleDef *>(JS_VALUE_GET_PTR(mod));
    auto ns = JS_GetModuleNamespace(js, m);
    defer(JS_FreeValue(js, ns));

    SPDLOG_TRACE("Reading game config");
    auto config = read_config(js, ns);
    if (!config) return Err(config.error());

    SPDLOG_TRACE("Reading game callbacks");
    auto load = js::try_get_property<std::optional<js::Function>>(js, ns, "load");
    if (!load) return Err(error::create("game.load is not a function"));
    auto update = js::try_get_property<std::optional<js::Function>>(js, ns, "update");
    if (!update) return Err(error::create("game.update is not a function"));
    auto draw = js::try_get_property<std::optional<js::Function>>(js, ns, "draw");
    if (!draw) return Err(error::create("game.draw is not a function"));
    auto pre_reload = js::try_get_property<std::optional<js::Function>>(js, ns, "preReload");
    if (!pre_reload) return Err(error::create("game.preReload is not a function"));
    auto post_reload = js::try_get_property<std::optional<js::Function>>(js, ns, "postReload");
    if (!post_reload) return Err(error::create("game.postReload is not a function"));

    SPDLOG_TRACE("Game created successfully");
    return Game({
        .js = js,
        .store = store,
        .config = *config,
        .load = std::move(*load),
        .update = std::move(*update),
        .draw = std::move(*draw),
        .pre_reload = std::move(*pre_reload),
        .post_reload = std::move(*post_reload),
    });
}

[[nodiscard]]
auto Game::config() const noexcept -> const GameConfig& {
    return _config;
}

[[nodiscard]]
auto Game::load() noexcept -> Result<> {
    if (!_load) return {};
    auto val = _load->operator()();
    JS_FreeValue(_js.get(), val);
    if (JS_HasException(_js.get())) return Err(error::from_js(_js.get(), JS_GetException(_js.get())));
    return {};
}

[[nodiscard]]
auto Game::update() noexcept -> Result<> {
    if (!_update) return {};
    auto val = _update.value()();
    JS_FreeValue(_js.get(), val);
    if (JS_HasException(_js.get())) return Err(error::from_js(_js.get(), JS_GetException(_js.get())));
    return {};
}

[[nodiscard]]
auto Game::draw() noexcept -> Result<> {
    if (!_draw) return {};
    auto val = _draw.value()();
    JS_FreeValue(_js.get(), val);
    if (JS_HasException(_js.get())) return Err(error::from_js(_js.get(), JS_GetException(_js.get())));
    return {};
}

[[nodiscard]]
auto Game::try_reload() noexcept -> Result<> try {
    std::optional<js::Value> state {};
    SPDLOG_TRACE("Reloading game");

    if (_pre_reload) {
        SPDLOG_TRACE("Game has preReload, so calling it");
        auto r = _pre_reload->operator()();
        if (JS_HasException(_js)) return Err(error::from_js(_js, JS_GetException(_js)));
        state = js::Value(_js, r);
    }

    SPDLOG_TRACE("Recreating game");
    auto game_result = Game::create(_js, _store);
    if (!game_result) return Err(game_result.error());
    auto new_game = std::move(*game_result);

    if (new_game._post_reload) {
        SPDLOG_TRACE("New game has postReload, so calling it");
        auto r = JS_UNINITIALIZED;
        if (state) {
            auto args = std::array {state->get()};
            r = new_game._post_reload->operator()(args);
        } else {
            r = new_game._post_reload->operator()();
        }
        if (JS_HasException(_js)) return Err(error::from_js(_js, JS_GetException(_js)));
        JS_FreeValue(_js, r);
    }

    SPDLOG_TRACE("Successfully reloaded game");
    *this = std::move(new_game);
    return {};
} catch (std::exception& e) {
    return Err(error::create(e.what()));
}

Game::Game(InitParams p) noexcept :
    _js(p.js),
    _store(p.store),
    _config(std::move(p.config)),
    _load(std::move(p.load)),
    _update(std::move(p.update)),
    _draw(std::move(p.draw)),
    _pre_reload(std::move(p.pre_reload)),
    _post_reload(std::move(p.post_reload)) {}

extern "C" auto module_loader(JSContext *ctx, const char *module_name, void *opaque) noexcept -> JSModuleDef * {
    SPDLOG_DEBUG("Loading module {}", module_name);
    auto e = static_cast<Engine *>(opaque);
    if (auto mod = e->load_module(module_name)) {
        return *mod;
    } else {
        JS_ThrowPlainError(ctx, "%s", std::format("{}", mod.error()->msg()).c_str());
        return nullptr;
    }
}

auto read_config(JSContext *js, JSValue ns) -> Result<GameConfig> {
    GameConfig config {};

    auto obj_result = js::try_get_property<std::optional<JSValue>>(js, ns, "config");
    if (!obj_result) return Err(error::from_js(js, obj_result.error()));
    auto obj_opt = *obj_result;
    if (!obj_opt) return config;
    auto obj = *obj_opt;
    defer(JS_FreeValue(js, obj));

    if (!JS_IsObject(obj)) {
        return Err(error::create("Game config must be object"));
    }

    if (auto title = js::try_get_property<std::string>(js, obj, "title")) config.title = *title;
    else return Err(error::from_js(js, title.error()));

    // TODO: other config fields

    return config;
}

} // namespace muen

#include "./engine/audio.cpp"
#include "./engine/music.cpp"
#include "./engine/sound.cpp"
#include "./engine/window.cpp"
