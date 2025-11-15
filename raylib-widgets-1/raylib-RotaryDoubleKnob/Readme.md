```markdown
# Dual Vernier Knob (DV_Knob) Usage Guide

This document explains how to integrate and use the **Dual Vernier Knob** code in your C program, which provides a graphical component with two concentric rotary knobs (outer and inner) for precise user input with vernier scale behavior.

---

## Overview

The Dual Vernier Knob component consists of:

- Two concentric rotary knobs (outer and inner).
- The inner knob rotates 10 times faster than the outer knob (controlled by a constant).
- User can interact with each knob independently by mouse dragging.
- The current combined value is tracked as a 64-bit integer (`int64_t`).
- The knobs display their current rotation angles visually with colored indicators.

---

## Files Included

- `dv_knob.h` — Declares the `DualVernierKnob` structure and API functions.
- `dv_knob.c` — Implements the initialization, rendering, input handling, and updating logic for the knobs.
- Usage requires linking with [raylib](https://www.raylib.com/) (for graphics and input).

---

## Initializing the Knob

```
DualVernierKnob knob = InitDualVernierKnob(centerX, centerY, outerRadius);
```

- `centerX`, `centerY` specify the screen coordinates of the knob's center.
- `outerRadius` is the radius of the outer knob; the inner knob radius is set automatically to approx. 45% of outer radius.
- The initial rotation angles for both knobs start at zero (pointing upwards).

---

## Rendering the Knob

Call in your frame drawing routine:

```
DrawDualVernierKnob(knob);
```

- Draws the two knobs with shaded circles.
- Adds colored indicator lines (blue for outer knob, red for inner knob) showing current knob rotation.
- Displays the current numeric value next to the knobs.

---

## Updating the Knob Input and Value

Call once per frame, passing a pointer to the knob and whether it should be active:

```
knob.value = UpdateDualVernierKnob(&knob, isActive);
```

- `isActive` (bool): If `false`, disables user interaction and stops dragging.
- Updates knob angles based on mouse input.
- Handles mouse press, drag, and release logic.
- Calculates and returns the current numeric value (based on inner knob angle scaled so 360° = 100 units).

---

## Mouse Interaction

- Click and drag **inner knob** (smaller circle) to rotate it directly.
- Click and drag **outer knob** (larger circle excluding inner knob area) to rotate it, which simultaneously moves the inner knob proportionally.
- Only one knob can be dragged at a time.
- Angles wrap correctly over 360°.

---

## Constants and Configuration

- `INNER_TO_OUTER_RATIO` = 10: inner knob rotates 10 times faster than outer knob.
- Initial knob angles are zero pointing **up** (rotated -90° internally so 0° = vertical).
- Radii ratio fixed to ~0.45 between inner and outer knobs.

---

## Example Usage in Main Loop

```
int main() {
    InitWindow(400, 300, "Dual Vernier Knob Example");
    SetTargetFPS(60);

    DualVernierKnob vernierKnob = InitDualVernierKnob(200, 150, 60);

    while (!WindowShouldClose()) {
        vernierKnob.value = UpdateDualVernierKnob(&vernierKnob, true);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawDualVernierKnob(vernierKnob);
        DrawText("Click and drag inner/outer knob", 20, 20, 20, DARKGRAY);
        DrawText(TextFormat("Value: %lld", (long long)vernierKnob.value), 20, 240, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

---

## Integration Notes

- This code depends on the [raylib](https://www.raylib.com/) library for graphics, input, and text drawing.
- Ensure `raylib` is properly installed and linked to your project.
- You may customize knob size, colors, or the value calculation formula as needed.
- For Unicode font support, you may use `LoadFontUnicode` function from the provided utility for better multilingual text rendering.

---

## Summary

The Dual Vernier Knob offers a clean, interactive GUI element for precise control, similar to two vernier scales mechanically coupled. This improves user accuracy in selecting values by combining coarse (outer knob) and fine (inner knob) increments.

By including the header and source, initializing once, calling update each frame, and drawing each frame, you can seamlessly add this control into your raylib-based C projects with intuitive mouse interaction and smooth visual feedback.
```
