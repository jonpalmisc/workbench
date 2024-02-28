#include <iostream>
#include <string>

class Dish {
    std::string m_name;
    int m_price;

    // Pretty sure Clang and/or GCC won't actually compile-time initialize this
    // structure with only the above fields, but to ensure that doesn't happen,
    // this cached, formatted "menu line" is initialized in the constructor.
    std::string m_menu_string;

public:
    Dish(std::string name, int price)
    {
        m_name = name;
        m_price = price;

        m_menu_string = name + " ($" + std::to_string(price) + ")";
    }

    std::string menu_string() const
    {
        return m_menu_string;
    }
};

class Menu {
    std::string m_name;
    std::vector<Dish *> m_items;

public:
    template <class... DishPtrTy>
    Menu(std::string name, DishPtrTy... args)
        : m_name(std::move(name))
        , m_items({ args... })
    {
    }

    std::string const &name() const
    {
        return m_name;
    }

    std::vector<Dish *> const &items() const
    {
        return m_items;
    }
};

// First, the constructor for each of these should get called. The word "first"
// is used lightly here, since I'm pretty sure there's no guarantee that these
// need to be initialized first.
static Dish g_ravioli { "Ravioli", 8 };
static Dish g_cannoli { "Cannoli", 5 };
static Dish g_biscotti { "Biscotti", 3 };

// Finally, the actual menu should get constructed.
static Menu g_menu {
    "Il Ristorante", &g_ravioli, &g_cannoli, &g_biscotti
};

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::cout << g_menu.name() << "\n"
              << std::string(40, '-') << "\n";
    for (auto const item : g_menu.items())
        std::cout << "  " << item->menu_string() << "\n";

    return 0;
}
