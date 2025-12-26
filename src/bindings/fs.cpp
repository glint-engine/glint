#include "fs.hpp"

#include <cassert>
#include <fstream>
#include <sstream>

#include <raylib.h>

#include "bindings.hpp"
#include "engine.hpp"
#include "js.hpp"

namespace muen::bindings::fs {

auto define(js::State *j) -> void {
    define_global_function(
        j,
        [](js::State *j) {
            const char *file_path = js::tostring(j, 1);

            engine::Engine *e = static_cast<engine::Engine *>(js::getcontext(j));
            auto path = std::string {e->root_path};
            if (path[path.size() - 1] != '/') {
                path.push_back('/');
            }
            path.append(file_path);

            try {
                auto file = std::ifstream {path};
                file.exceptions(file.failbit | file.badbit);
                auto buf = std::stringstream {};
                buf << file.rdbuf();
                auto str = buf.str();
                js::pushstring(j, str.c_str());
            } catch (std::exception& e) {
                js::error(j, "Could not read %s: %s", file_path, strerror(errno));
            }
        },
        "__muenFsRead",
        1
    );

    define_global_function(
        j,
        [](js::State *j) {
            // NOTE: leaking some memory here
            const char *file_path = strdup(js::tostring(j, 1));

            engine::Engine *e = static_cast<engine::Engine *>(js::getcontext(j));
            assert(e->modules != nullptr);

            auto buf = std::stringstream {};

            if (auto m = e->modules->find(file_path); m != e->modules->end()) {
                buf << m->second;
            } else {
                auto path = std::string {e->root_path};
                if (path[path.size() - 1] != '/') {
                    path.push_back('/');
                }
                path.append(file_path);

                try {
                    auto file = std::ifstream {path};
                    file.exceptions(file.failbit | file.badbit);
                    buf << file.rdbuf();
                } catch (std::exception& e) {
                    js::error(j, "Could not read %s: %s", file_path, strerror(errno));
                }
            }

            auto str = buf.str();
            js::pop(j, 2);
            js::pushnull(j);
            js::pushstring(j, "module");
            js::pushstring(j, str.c_str());
            js::create_function(j, file_path);
        },
        "__muenFsLoad",
        1
    );
}

} // namespace muen::bindings::fs
