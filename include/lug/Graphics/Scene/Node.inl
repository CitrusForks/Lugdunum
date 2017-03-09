inline Node* Node::getNode(const std::string& name) {
    return static_cast<Node*>(::lug::Graphics::Node::getNode(name));
}

inline const Node* Node::getNode(const std::string& name) const {
    return static_cast<const Node*>(::lug::Graphics::Node::getNode(name));
}
