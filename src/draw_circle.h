#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>

ImVec2 LatLonToScreen(float lat, float lon, ImVec2 map_top_left, ImVec2 map_size, float zoom)
{
    // Assuming the map covers a specific geographic area
    // Replace these with the actual bounds of your map
 /*   float map_min_lat = -90.0f;
    float map_max_lat = 90.0f;
    float map_min_lon = -180.0f;
    float map_max_lon = 180.0f;*/

    float map_min_lat = -10.0f;
    float map_max_lat = 40.0f;
    float map_min_lon = -120.0f;
    float map_max_lon = 10.0f;

    // Normalize latitude and longitude to [0, 1]
    float norm_lat = (lat - map_min_lat) / (map_max_lat - map_min_lat);
    float norm_lon = (lon - map_min_lon) / (map_max_lon - map_min_lon);

    // Apply zoom factor
    float zoomed_width = map_size.x * zoom;
    float zoomed_height = map_size.y * zoom;

    // Scale to map size
    float x = map_top_left.x + norm_lon * zoomed_width;
    float y = map_top_left.y + (1.0f - norm_lat) * zoomed_height; // Invert y-axis

    return ImVec2(x, y);
}

// Function to convert latitude and longitude to screen coordinates
ImVec2 LatLonToScreen(float lat, float lon, ImVec2 map_top_left, ImVec2 map_size) 
{
    // Assuming the map covers a specific geographic area
    // Replace these with the actual bounds of your map
    float map_min_lat = -90.0f;
    float map_max_lat = 90.0f;
    float map_min_lon = -180.0f;
    float map_max_lon = 180.0f;

    // Normalize latitude and longitude to [0, 1]
    float norm_lat = (lat - map_min_lat) / (map_max_lat - map_min_lat);
    float norm_lon = (lon - map_min_lon) / (map_max_lon - map_min_lon);

    // Scale to map size
    float x = map_top_left.x + norm_lon * map_size.x;
    float y = map_top_left.y + (1.0f - norm_lat) * map_size.y; // Invert y-axis

    return ImVec2(x, y);
}

void DrawCircleAtLatLon(float lat, float lon) 
{
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 map_top_left = window_pos;
    ImVec2 map_size = window_size;

    ImVec2 circle_pos = LatLonToScreen(lat, lon, map_top_left, map_size);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircle(circle_pos, 10.0f, IM_COL32(255, 0, 0, 255), 0, 2.0f);
    //+++green color
    //draw_list->AddCircleFilled(circle_pos, 10.0f, IM_COL32(0, 255, 0, 255));
}

void DrawCircleAtLatLon_Color(float lat, float lon, ImU32 color)
{
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 map_top_left = window_pos;
    ImVec2 map_size = window_size;


     ImVec2 circle_pos = LatLonToScreen(lat, lon, map_top_left, map_size, 2.5);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //draw_list->AddCircle(circle_pos, 10.0f, IM_COL32(255, 0, 0, 255), 0, 2.0f);
    draw_list->AddCircle(circle_pos, 10.0f, color, 0, 2.0f);
    //+++green color
    //draw_list->AddCircleFilled(circle_pos, 10.0f, IM_COL32(0, 255, 0, 255));
}

// In your ImGui rendering loop
//ImGui::Begin("Map Window");
//DrawCircleAtLatLon(37.7749f, -122.4194f); // Example: San Francisco coordinates
//ImGui::End();


//========================================

// Your transformation functions
ImVec2 LatLonToXY(float lat, float lon) 
{
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 map_top_left = window_pos;
    ImVec2 map_size = window_size;

   // Assuming the map covers a specific geographic area
   // Replace these with the actual bounds of your map
    float map_min_lat = -90.0f;
    float map_max_lat = 90.0f;
    float map_min_lon = -180.0f;
    float map_max_lon = 180.0f;

    // Normalize latitude and longitude to [0, 1]
    float norm_lat = (lat - map_min_lat) / (map_max_lat - map_min_lat);
    float norm_lon = (lon - map_min_lon) / (map_max_lon - map_min_lon);

    // Scale to map size
    //float x = map_top_left.x + norm_lon * map_size.x;
    //float y = map_top_left.y + (1.0f - norm_lat) * map_size.y; // Invert y-axis
    float x = map_top_left.x;
    float y = map_top_left.y;

    // Implement your transformation logic here
    return ImVec2(x, y);
    //return ImVec2(lat, lon);
}

// Function to set the viewport based on the corners
void SetViewport(ImVec2 topLeft, ImVec2 bottomRight) 
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    // Calculate the new viewport position
    ImVec2 newViewportPos = topLeft;

    ImVec2 newViewportPos_2 = bottomRight;

    // Set the viewport position
    window->Scroll.x = newViewportPos.x;
    window->Scroll.y = newViewportPos.y;

    // Optionally, adjust the zoom level if needed
    //auto zoomLevel = 2.05;
    //window->FontWindowScale = zoomLevel;

    // Draw your circle or other elements
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircle(ImVec2(window->Scroll.x, window->Scroll.y), 20, IM_COL32(255, 0, 0, 255));
    draw_list->AddCircle(newViewportPos_2, 20, IM_COL32(0, 0, 0, 255));
}

