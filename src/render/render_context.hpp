#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include <string>
#include <vector>

struct RenderedLink {
    std::string url;
    double x, y, width, height;
};

struct RenderContext {
    std::vector<RenderedLink> links;
    
    // For inherited styles
    bool in_link = false;
    std::string current_href = "";
};

#endif // RENDER_CONTEXT_HPP
