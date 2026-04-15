# Castle of No Return — Claude Coding Rules

## 1. Always use braces on if statements

Prefer:
```cpp
if (!map_is_walkable(&g->map, ex, ey)) {
    continue;
}
```

Never:
```cpp
if (!map_is_walkable(&g->map, ex, ey)) continue;
```

This applies to all control flow: `if`, `else`, `for`, `while`.

---

## 2. Single space around operators — no alignment padding

Prefer:
```cpp
int a = 5;
int bc = 10;
int def = 15;
```

Never:
```cpp
int a   = 5;
int bc  = 10;
int def = 15;
```

This applies to variable declarations, struct field assignments, and all `=` usage.

---

## 3. Function signatures on one line

Prefer:
```cpp
static void handle_slot_result(SlotResult result, int slot, int slot_is_save) {
```

Never:
```cpp
static void handle_slot_result(SlotResult result, int slot,
    int slot_is_save) {
```

If a signature is too long, shorten the parameter names rather than wrapping.

---

## 4. Minimal code per change set

- Provide the smallest possible change per step
- Wait for an explicit "next step" prompt before providing the next change
- Never combine multiple features into one code block
- Never add unrequested changes alongside a requested change

---

## 5. Explain each change

For every code change, briefly explain:
- What the change does
- Why it is needed
- Any side effects or dependencies

---

## 6. Be maximally specific about code location

When instructing where to add or modify code, always specify:
- The exact filename and path
- The exact function name
- The exact surrounding lines to find the insertion point

Prefer:

> In `src/core/systems/ai_system.cpp` in `tickMarches` replace:
> ```cpp
> world.battle.playerMaxHealth = defStr * 10;
> ```
> with:
> ```cpp
> world.battle.playerMaxHealth = defStr * 5;
> ```

Never:

> Add this after the combat completed block.

---

## 7. All game state changes must be reflected in save/load once implemented.

Once implemented, any new field added to any struct that is part of persistent game state must also be:
- Serialized in `save_game()` in `src/core/systems/save_load.c`
- Deserialized in `load_game()` in `src/core/systems/save_load.c`

Omitting this causes data to silently reset to zero on load.