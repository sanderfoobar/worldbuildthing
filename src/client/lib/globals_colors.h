#pragma once
#include <ranges>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <cctype>

#include <QString>

namespace colors {
  inline const std::map<std::string, const char*> all = {
    {"color_active", "#4866C5"},
    {"color_highlight", "#4D92B6"},
    {"color_btn_highlight", "#6A84D9"},
    {"color_border", "#346077"},
    {"color_grey", "#171C38"},
    {"color_bg", "#101428"},
    {"color_bg_light", "#232A4E"},
  };

  inline std::string hex_to_rgb_string(const std::string &hex) {
    if (hex.empty())
      return "rgb(0,0,0)";
    if (hex[0] == '#') {
      unsigned int r, g, b;
      if (hex.length() == 7) {
        std::sscanf(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
        return "rgb(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ")";
      }
    }
    // fallback for named colors (simplified)
    if (hex == "red")
      return "rgb(255,0,0)";
    if (hex == "green")
      return "rgb(0,255,0)";
    if (hex == "blue")
      return "rgb(0,0,255)";
    return "rgb(0,0,0)";
  }

  inline QString injectStyleSheetColors(QString& stylesheet) {
    for (const auto& [name, hex] : all) {
      auto _name = "$" + QString::fromStdString(name);
      stylesheet.replace(_name, QString::fromStdString(hex));
    }
    return stylesheet;
  }
}
