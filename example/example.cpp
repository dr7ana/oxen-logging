#include <oxen/log.hpp>


int main() {
    using namespace oxen::log;

    add_sink(oxen::log::Type::Print, "stdout");
    add_sink(oxen::log::Type::File, "foo.log");

    auto cat_foo = Cat("foo");
    auto cat_bar = Cat("bar");

    cat_foo->set_level(Level::warn);
    cat_bar->set_level(Level::trace);

    trace(cat_foo, "hello {}", 42);
    debug(cat_foo, "hello {}", 42);
    info(cat_foo, "hello {}", 42);
    warning(cat_foo, "hello {}", 42);
    error(cat_foo, "hello {}", 42);
    critical(cat_foo, "hello {}", 42);

    trace(cat_bar, "hello {}", 42);
    debug(cat_bar, "hello {}", 42);
    info(cat_bar, "hello {}", 42);
    warning(cat_bar, "hello {}", 42);
    error(cat_bar, "hello {}", 42);
    critical(cat_bar, "hello {}", 42);

    reset_level(Level::warn);
    critical(cat_bar, "hello {}", 42);
    critical(cat_foo, "hello {}", 42);
    info(cat_foo, "hello {}", 42);
    info(cat_foo, "hello {}", 42);

    set_level(cat_bar, Level::debug);
    info(cat_foo, "hello {}", 42);
    info(cat_bar, "hello {}", 42);
}
