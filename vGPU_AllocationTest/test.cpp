#include <string>
#include <vector>

#include "pugixml.hpp"
using namespace pugi;

int main() {
    xml_document doc;
    xml_parse_result result = doc.load_file("vGPUTest/example2.xml");
    xml_node _messages = doc.child("MyApp").child("Messages");
    std::vector<std::pair<std::string, std::string> > vecNode;

    // Welcome Farewell
    for (xml_node _plugin = _messages.first_child(); _plugin; _plugin = _plugin.next_sibling()) {
        typedef std::pair<std::string, std::string> string_Pair;
        std::string strNodeName = _plugin.name();
        std::string strValue = _messages.child_value(strNodeName.c_str());
        vecNode.push_back(string_Pair(strNodeName, strValue));
    }

    xml_node _window = doc.child("MyApp").child("Windows");
    for (int i = 0; i < vecNode.size(); i++) {
        // Welcome Farewell
        xml_node new_node = _window.append_child(vecNode[i].first.c_str());
        new_node.append_child(node_pcdata).set_value(vecNode[i].second.c_str());

        // param
        xml_node param = _window.insert_child_before("param", new_node);
        param.append_attribute("name") = "version";
        param.append_attribute("value") = 1.1;
        param.insert_attribute_after("type", param.attribute("name")) = "float";
    }

    doc.save_file("vGPUTest/modified_example2.xml");
    getchar();
    return 0;
}