#ifndef MANA_COMPONENTDESCRIPTION_HPP
#define MANA_COMPONENTDESCRIPTION_HPP

struct ComponentDescription {
    std::string sourceFile; // The header file which contains the source code of the component definition.
    std::string typeName; // The name of the component type
    std::string iField; // Describe members by name and type
};

#endif //MANA_COMPONENTDESCRIPTION_HPP
