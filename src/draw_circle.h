#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>

struct LatLonPoint {
    float lat, lon;
};

ImVec2 LatLonToScreen(float lat, float lon, ImVec2 map_top_left, ImVec2 map_size, float zoom, ImVec2 offset) {
    const float map_min_lat = 33.0172f;
    const float map_max_lat = 33.20826f;
    const float map_min_lon = -117.20900f;//-117.19900f;
    const float map_max_lon = -117.01439f;
    
    float norm_lat = (lat - map_min_lat) / (map_max_lat - map_min_lat);
    float norm_lon = (lon - map_min_lon) / (map_max_lon - map_min_lon);

    float zoomed_width = map_size.x * zoom;
    float zoomed_height = map_size.y * zoom;
    float x = map_top_left.x + norm_lon * zoomed_width + offset.x;
    float y = map_top_left.y + (1.0f - norm_lat) * zoomed_height + offset.y;

    return ImVec2(x, y);
}


void CalculateZoomAndOffsetForMultiplePoints(const std::vector<LatLonPoint>& points, ImVec2 map_size, float& out_zoom, ImVec2& out_offset) {
    if (points.empty()) {
        out_zoom = 1.0f;
        out_offset = ImVec2(0.0f, 0.0f);
        return;
    }

    float min_lat = points[0].lat;
    float max_lat = points[0].lat;
    float min_lon = points[0].lon;
    float max_lon = points[0].lon;

    // Calculate the bounding box for the points
    // Find the bounding box for the points
    for (const auto& point : points) {
        min_lat = std::min(min_lat, point.lat);
        max_lat = std::max(max_lat, point.lat);
        min_lon = std::min(min_lon, point.lon);
        max_lon = std::max(max_lon, point.lon);
    }

    // Apply different padding factors to latitude and longitude
    float padding_lat = 0.005f * (max_lat - min_lat);
    float padding_lon = 0.005f * (max_lon - min_lon);

    float padded_min_lat = min_lat - padding_lat;
    float padded_max_lat = max_lat + padding_lat;
    float padded_min_lon = min_lon - padding_lon;
    float padded_max_lon = max_lon + padding_lon;

    // Calculate zoom values for both latitude and longitude, with a cap to prevent excess zoom
    float zoom_x = map_size.x / (padded_max_lon - padded_min_lon);
    float zoom_y = map_size.y / (padded_max_lat - padded_min_lat);
    out_zoom = std::min(zoom_x, zoom_y);

    // Cap the zoom to a maximum reasonable level
    float max_zoom = 5.0f;
    out_zoom = std::min(out_zoom, max_zoom);
 
    // Calculate the center point of the bounding box for offset
    float center_lat = (padded_max_lat + padded_min_lat) / 2.0f;
    float center_lon = (padded_max_lon + padded_min_lon) / 2.0f;

    // Convert this center point to screen coordinates with the calculated zoom
    ImVec2 center_screen = LatLonToScreen(center_lat, center_lon, ImVec2(0.0f, 0.0f), map_size, out_zoom, ImVec2(0.0f, 0.0f));
    
    // Set offset to center bounding box with additional offset to separate close points
    out_offset = ImVec2((map_size.x / 2.0f) - center_screen.x, (map_size.y / 2.0f) - center_screen.y);
}

//
//void DrawCircleAtLatLon(float lat, float lon, ImU32 color, float zoom, ImVec2 offset) {
//    ImVec2 window_pos = ImGui::GetWindowPos();
//    ImVec2 window_size = ImGui::GetWindowSize();
//    ImVec2 map_top_left = window_pos;
//    ImVec2 map_size = window_size;
//    ImVec2 circle_pos = LatLonToScreen(lat, lon, map_top_left, map_size, zoom, offset);
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    draw_list->AddCircle(circle_pos, 5.0f, color, 0, 2.0f);
//}


void DrawPoints(const std::vector<LatLonPoint>& points, ImU32 color = IM_COL32(0, 255, 0, 255), bool enable_flash_ligh = false) {
    float zoom;
    ImVec2 offset;
    CalculateZoomAndOffsetForMultiplePoints(points, ImGui::GetWindowSize(), zoom, offset);

    // Minimum screen distance to separate close points
    const float min_separation_distance = 15.0f;  // Adjust for spacing needs

    std::vector<ImVec2> screen_positions;

    ImU32 inner_flashing_color;
    if (enable_flash_ligh) {
        // Track elapsed time for flashing effect
        static auto start_time = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        float elapsed_time = std::chrono::duration<float>(now - start_time).count(); // Time in seconds

        // Calculate the flashing color
        float flashing_color_value = (sin(elapsed_time * 3.0f) + 1.0f) / 2.0f; // Oscillates between 0 and 1
        //ImU32 flashing_color = IM_COL32(255 * flashing_color_value, 0, 0, 255); // Flashing red color
        //ImU32 inner_flashing_color = IM_COL32(0, 255 * flashing_color_value, 0, 255); // Flashing green color
        inner_flashing_color = IM_COL32(0, 255 * flashing_color_value, 0, 255); // Flashing green color
    }

    for (const auto& point : points) {
        ImVec2 screen_pos = LatLonToScreen(point.lat, point.lon, ImGui::GetWindowPos(), ImGui::GetWindowSize(), zoom, offset);

        // Check if this point overlaps with any previously drawn points
        for (const auto& other_pos : screen_positions) {
            float dist = std::sqrt(std::pow(screen_pos.x - other_pos.x, 2) + std::pow(screen_pos.y - other_pos.y, 2));

            // If too close, apply radial separation offset
            if (dist < min_separation_distance && dist > 0) {
                float angle = atan2(screen_pos.y - other_pos.y, screen_pos.x - other_pos.x);
                screen_pos.x += min_separation_distance * cos(angle);
                screen_pos.y += min_separation_distance * sin(angle);
            }
        }

        screen_positions.push_back(screen_pos);

        // Draw the outer solid circle
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddCircleFilled(screen_pos, 5.0f, color); // Solid outer circle

        if (enable_flash_ligh){
            // Draw the circle at the adjusted position with flashing color
            // Draw the inner flashing circle
            draw_list->AddCircleFilled(screen_pos, 4.0f, inner_flashing_color); // Flashing inner circle
        }
    }
}
