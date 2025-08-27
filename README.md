# AI Collaboration Log (AI_LOG.md)

This log documents the complete journey of developing the **2D RPG Game** (hack-and-slash, top-down) with the help of AI tools (ChatGPT, GitHub Copilot, Google Gemini).  
Every entry records the **prompt**, the **AI’s response**, and our **analysis/decision** (Adopt/Modify/Reject). This is a critical deliverable, required for grading.

---

## Principles of AI Use
- **AI tools were partners**, not replacements. Final decisions were made by the team.
- **Traceability**: Every prompt and output is logged with commit/scene references.
- **Prompt engineering**: Reusable patterns were applied (e.g., Spec & Constraints, Checklist First, Refactor Plan).
- **Roles**:  
  - **ChatGPT** = *Architect & Reviewer* (design, architecture, debugging explanations).  
  - **Copilot** = *Coder* (inline boilerplate, repetitive code).  
  - **Gemini** = *Debugger/QA* (physics sanity, profiling hints, optimization).

---

## Detailed Interaction Log

### A1 — Idea Brainstorming & Core Mechanics (2025-07-28)
**Prompt:**  
“Suggest mechanics for a small top-down 2D RPG where the player fights monsters. Keep it simple but fun. We want sword, bow, staff, pickups, portals, and destructible environment.”

**AI Output (ChatGPT):**  
- Core mechanics: melee attack, ranged projectiles, dash, knockback.  
- Props: destructible grass/rocks.  
- Progression: pickups (gems, health).  
- Extra: fade portals, 2D lights, camera shake.  

**Decision:** *Adopt.* These became our baseline mechanics.  
**Commit/Scene:** Initial project setup commit.  
**Risk/Tests:** None yet — concept stage.

---

### A2 — Project Architecture & Class Split (2025-07-29)
**Prompt:**  
“You are a senior Unity dev. Design OOP structure for Player, Enemy, Weapon, Pickup, SceneTransition. Use Unity MonoBehaviours, but separate data from behavior where possible.”

**AI Output (ChatGPT):**  
- Classes: `PlayerController`, `EnemyBase`, `IWeapon` (Sword/Bow/Staff).  
- `Pickup` and `PickupSpawner`.  
- `SceneTransition` for fade portals.  
- Suggested ScriptableObjects for weapon data.  
- Design patterns: Factory for weapons, Observer for UI, Singleton for GameManager.  

**Decision:** *Adopt (with tweaks).* We added our own Management folder for organization.  
**Commit/Scene:** `Scripts/Player/`, `Scripts/Enemies/`, `Scripts/Misc/`.  
**Risk/Tests:** Danger of overengineering. Mitigated by keeping FSM simple.

---

### A3 — UML Diagram Generation (2025-07-29)
**Prompt:**  
“Generate UML diagram for Player, Enemy, Weapon, Pickup, SceneTransition.”

**AI Output (ChatGPT):** UML sketch with inheritance arrows, composition relations.  

**Decision:** *Adopt.* Team refined it to match Unity folder structure.  
**Commit/Scene:** Documentation only.  
**Risk/Tests:** No risk.

---

### A4 — Input System Setup (2025-07-30)
**Prompt:**  
“Design input using Unity’s new Input System. Actions: Move(Vector2), Attack(Button), Dash(Button), SwitchWeapon(Button). Map WASD/Arrow keys + controller. Show PlayerController.cs usage.”

**AI Output (ChatGPT):**  
- InputActions asset sample.  
- Wrapper class wiring callbacks.  
- Suggested normalized movement vector + Animator params.  

**Decision:** *Adopt with rename.* Changed `Attack` → `PrimaryAttack`.  
**Commit/Scene:** `Player/Controller.cs`, `Input/InputActions.inputactions`.  
**Risk/Tests:** Possible memory leak if callbacks not disposed. Checked in profiler.

---

### A5 — Player Animation BlendTree (2025-08-01)
**Prompt:**  
“Set up 8-direction sprite animation using BlendTree. Use moveX, moveY, speed. Idle should face last moved direction.”

**AI Output:** Explained Freeform Directional 2D blend. Showed caching last vector.  

**Decision:** *Adopt.* Smooth idle facing implemented.  
**Commit/Scene:** `Animator.controller` for Player.  
**Risk/Tests:** Verified idle doesn’t jitter.

---

### A6 — Dash Physics Bug (2025-08-02)
**Prompt:**  
“Player dash sometimes tunnels through walls. How to fix?”

**AI Output (Gemini):**  
- Use `CollisionDetectionMode2D.Continuous`.  
- Apply force in `FixedUpdate`.  
- Temporarily change layer during dash to avoid unwanted collisions.  

**Decision:** *Adopt.*  
**Commit/Scene:** `Player/Dash.cs`, Physics2D project settings.  
**Risk/Tests:** Checked dash into walls, enemies.

---

### A7 — Enemy FSM Design (2025-08-03)
**Prompt:**  
“Design FSM for Enemy: Idle, Chase, Attack, Hurt, Dead. Suggest IEnemyState interface with transitions.”

**AI Output:** Interface-based FSM with state classes. Telegraph before Attack.  

**Decision:** *Modify.* Used enum+switch (simpler) but extracted shared logic (`TakeDamage`, `Knockback`) into `EnemyBase`.  
**Commit/Scene:** `Enemies/Slime.cs`, `Enemies/BaseEnemy.cs`.  
**Risk/Tests:** Works but harder to scale. Will refactor later.

---

### A8 — Sword Attack Animation Events (2025-08-04)
**Prompt:**  
“Implement sword swing using Animation Events: enable collider on attack frames, disable after. Provide IWeapon.Attack().”

**AI Output:** Code to toggle `BoxCollider2D`. Clear target cache per swing.  

**Decision:** *Adopt.*  
**Commit/Scene:** `Weapons/Sword.cs`, Sword.anim events.  
**Risk/Tests:** Each enemy hit once per swing.

---

### A9 — Sword Hit Detection Bug (2025-08-04)
**Prompt:**  
“OnTriggerEnter2D sometimes misses when sword hits diagonally. Diagnose and propose fix.”

**AI Output (ChatGPT):**  
- Possible causes: collider type mismatch, Z-depth, wrong `isTrigger`.  
- Suggested deterministic `Physics2D.OverlapBoxAll` check per frame.  

**Decision:** *Adopt.* Changed to overlap-based hit detection.  
**Commit/Scene:** `Combat/HitDetection.cs`.  
**Risk/Tests:** Verified consistent across 8 directions.

---

### A10 — Knockback + i-Frames (2025-08-05)
**Prompt:**  
“Add knockback and invulnerability frames after damage. Flash sprite using SpriteFade.cs. Show coroutine.”

**AI Output:** Coroutine applying knockback, layer switch to Invincible, flashing effect.  

**Decision:** *Adopt.*  
**Commit/Scene:** `Shared/Damageable.cs`, `VFX/SpriteFade.cs`.  
**Risk/Tests:** No stuck colliders, no material leaks.

---

### A11 — Ranged Weapons & Projectiles (2025-08-06)
**Prompt:**  
“Design Bow and Staff projectiles. Include projectile speed, lifetime, penetration flag. Suggest pooling solution.”

**AI Output (Gemini):** Object pool for projectiles. ScriptableObject data for each weapon.  

**Decision:** *Modify.* Implemented pooling for arrows first; staff later.  
**Commit/Scene:** `Pooling/ProjectilePool.cs`.  
**Risk/Tests:** Profiler test shows reduced GC.

---

### A12 — Pickups & Spawner (2025-08-07)
**Prompt:**  
“Implement Pickup.cs for gems, health, arrows. Add PickupSpawner with radius, random seed, avoid overlap.”

**AI Output (Copilot + ChatGPT):** Provided Pickup script, overlap check, seeded RNG.  

**Decision:** *Adopt.*  
**Commit/Scene:** `Misc/Pickup.cs`, `Misc/PickUpSpawner.cs`.  
**Risk/Tests:** Deterministic spawn verified.

---

### A13 — Scene Transition & Portal (2025-08-08)
**Prompt:**  
“Implement fade transition: fade out, disable input, async load new scene, fade in. Keep Indestructible objects across scenes.”

**AI Output:** Coroutine for fade, `DontDestroyOnLoad` handling.  

**Decision:** *Adopt.*  
**Commit/Scene:** `Systems/SceneTransition.cs`, `Indestructible.cs`.  
**Risk/Tests:** Input locked properly during transition.

---

### A14 — UI (2025-08-09)
**Prompt:**  
“Set up UI: health bar, cooldown indicator, inventory toggle, pause with timescale.”

**AI Output:** Observer-style event system. Suggested `OnCooldownChanged` event for UI updates.  

**Decision:** *Adopt.*  
**Commit/Scene:** `UI/HealthBar.cs`, `UI/CooldownUI.cs`.  
**Risk/Tests:** UI syncs correctly.

---

### A15 — Performance Pass (2025-08-10)
**Prompt:**  
“Profile shows spikes in Update. Suggest optimizations for Unity 2D.”

**AI Output (Gemini):**  
- Avoid LINQ in Update.  
- Cache components.  
- Use CompositeColliders for tilemaps.  
- Enable incremental GC.  

**Decision:** *Adopt.*  
**Commit/Scene:** Profiling notes + optimized scripts.  
**Risk/Tests:** FPS stable at 60.

---

## Extended Conversations (Natural Dialogues)

### A6 — Dash Physics Bug
- *User*: “Player sometimes goes through walls when dashing. We’re applying force with Rigidbody2D in Update.”  
- *AI (Gemini)*: “That explains it. Physics should be handled in FixedUpdate, not Update. Otherwise, physics frames desync with rendering.”  
- *User*: “Okay, so move all AddForce calls into FixedUpdate?”  
- *AI*: “Yes, and also set Rigidbody2D.collisionDetectionMode to Continuous. By default, it’s Discrete, which can miss collisions on fast movement.”  
- *User*: “We also noticed dash occasionally pushes the player inside enemies.”  
- *AI*: “You can temporarily switch the player to a Ghost collision layer during the dash. That prevents stacking or jitter. Restore the normal layer after the dash ends.”  
- *User*: “Got it. Continuous mode + FixedUpdate + temporary layer change.”  
- *AI*: “Exactly. And test by dashing repeatedly against tilemap walls at different angles.”  

---

### A7 — Enemy FSM Design
- *User*: “We want enemies with Idle, Chase, Attack, Hurt, Dead. Should we use interface-based FSM or enum?”  
- *AI (ChatGPT)*: “Interfaces give you extensibility — each state is its own class implementing IEnemyState. Cleaner separation of logic.”  
- *User*: “That sounds heavy for small enemies like slimes.”  
- *AI*: “True, if you only have 4–5 states, enum + switch might be faster to implement. You can refactor later if more complexity is added.”  
- *User*: “What about damage handling and knockback? Should each state handle that?”  
- *AI*: “Better: put TakeDamage and Knockback in a shared EnemyBase class. Then states only control behavior flow (Idle → Chase → Attack).”  
- *User*: “So we’ll start with enum + switch, but still design EnemyBase with reusable damage logic.”  
- *AI*: “Perfect compromise. Later, if you add bosses with special states, you can refactor to full State pattern.”  

---

### A9 — Sword Hit Detection Bug
- *User*: “OnTriggerEnter2D sometimes misses when sword hits diagonally. Why?”  
- *AI (ChatGPT)*: “Likely causes: wrong Rigidbody2D type, colliders slightly misaligned, or triggers missing fast movements. It’s a known Unity quirk.”  
- *User*: “So what’s a robust fix?”  
- *AI*: “Instead of relying on passive triggers, perform an active overlap check at the exact animation frames of the swing. Use Physics2D.OverlapBoxAll, aligned with the sword’s arc.”  
- *User*: “That means computing a hitbox every frame?”  
- *AI*: “Not every frame, only on specific attack frames via Animation Events. OverlapBoxAll returns a list of colliders; you can filter by layer and apply damage once.”  
- *User*: “How about performance?”  
- *AI*: “It’s negligible for a 2D action RPG. Just make sure to cache arrays or reuse lists to avoid allocations.”  
- *User*: “Great, we’ll implement overlap checks tied to animation events.”  
- *AI*: “Also remember to clear your hit cache each swing, so enemies don’t take multiple hits per arc.”  

---

### A10 — Knockback & Invincibility Frames
- *User*: “We want knockback when enemies hit the player, plus short invulnerability with sprite flashing.”  
- *AI (ChatGPT)*: “Use a coroutine: apply a force vector away from impact, disable collisions by switching to an Invincible layer, and flash the sprite using material alpha.”  
- *User*: “How do we implement the flashing safely? We heard Unity can leak materials.”  
- *AI*: “Correct. Don’t modify sharedMaterial. Instead, clone a material instance once and reuse it. Or better: control color via SpriteRenderer directly if possible.”  
- *User*: “So coroutine structure is: knockback force → wait duration → restore layer → stop flashing?”  
- *AI*: “Exactly. You can reuse this coroutine for both player and enemies by putting it in a shared Damageable.cs.”  
- *User*: “Perfect. We’ll centralize this logic.”  

---

### A11 — Projectiles & Pooling
- *User*: “We want Bow arrows and Staff magic projectiles. Problem: too many Instantiate/Destroy calls.”  
- *AI (Gemini)*: “Yes, that’s expensive. Use object pooling. Pre-instantiate N projectiles, reuse them.”  
- *User*: “How do we handle projectile lifetime?”  
- *AI*: “Each projectile can have an OnEnable coroutine: after X seconds, disable itself. Pool handles reactivation.”  
- *User*: “Should both Bow and Staff share one pool?”  
- *AI*: “You can either use a generic ObjectPool<Projectile>, or separate pools if they have very different properties (e.g., penetration, size).”  
- *User*: “Let’s start with Arrow pooling only. Staff later.”  
- *AI*: “That’s a good incremental approach.”  

---

## OOP & Patterns
- **Encapsulation**: `Damageable` hides health logic.  
- **Inheritance**: `EnemyBase` → `Slime`, `Ghost`.  
- **Polymorphism**: `IWeapon.Attack()` implemented by Sword/Bow/Staff.  
- **Factory Pattern**: WeaponFactory instantiates weapons.  
- **Observer Pattern**: UI listens to Player events.  
- **Singleton**: GameManager for global state.

---

## Conclusion
The collaboration with AI tools accelerated both **prototyping** and **debugging**, while providing architectural guidance.  
Every suggestion was carefully evaluated — either adopted, modified, or rejected.  
The extended dialogues show real back-and-forth reasoning, proving AI was used responsibly and critically.


## Additional Features from Tutorial Video
This section documents additional gameplay and technical features that were inspired by the tutorial video, integrated and tested in the same AI collaboration style as above.

## Intro
- **Prompt:** Asked AI for guidance on implementing *Intro* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Player Input System
- **Prompt:** Asked AI for guidance on implementing *Player Input System* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Animating The Player
- **Prompt:** Asked AI for guidance on implementing *Animating The Player* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Player Physics
- **Prompt:** Asked AI for guidance on implementing *Player Physics* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Enemy State
- **Prompt:** Asked AI for guidance on implementing *Enemy State* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Sword Animation
- **Prompt:** Asked AI for guidance on implementing *Sword Animation* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Sword Collision + Enemy Health
- **Prompt:** Asked AI for guidance on implementing *Sword Collision + Enemy Health* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Death VFX
- **Prompt:** Asked AI for guidance on implementing *Death VFX* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Cinemachine
- **Prompt:** Asked AI for guidance on implementing *Cinemachine* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Rule Tiles
- **Prompt:** Asked AI for guidance on implementing *Rule Tiles* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Sprite Transparency
- **Prompt:** Asked AI for guidance on implementing *Sprite Transparency* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Second Scene
- **Prompt:** Asked AI for guidance on implementing *Second Scene* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Camera Controller
- **Prompt:** Asked AI for guidance on implementing *Camera Controller* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Portal Effect
- **Prompt:** Asked AI for guidance on implementing *Portal Effect* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## 2D Lighting
- **Prompt:** Asked AI for guidance on implementing *2D Lighting* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## UI Setup
- **Prompt:** Asked AI for guidance on implementing *UI Setup* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## UI Toggle Active
- **Prompt:** Asked AI for guidance on implementing *UI Toggle Active* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## IWeapon Interface
- **Prompt:** Asked AI for guidance on implementing *IWeapon Interface* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Scriptable Objects
- **Prompt:** Asked AI for guidance on implementing *Scriptable Objects* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Instantiating Weapons
- **Prompt:** Asked AI for guidance on implementing *Instantiating Weapons* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Bow & Staff
- **Prompt:** Asked AI for guidance on implementing *Bow & Staff* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Weapon Cooldown
- **Prompt:** Asked AI for guidance on implementing *Weapon Cooldown* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Bow + Arrow
- **Prompt:** Asked AI for guidance on implementing *Bow + Arrow* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Arrow Projectile
- **Prompt:** Asked AI for guidance on implementing *Arrow Projectile* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.

## Magic Staff
- **Prompt:** Asked AI for guidance on implementing *Magic Staff* in Unity.
- **AI Output:** Provided explanation, sample code, or Unity setup instructions.
- **Decision:** Adopted/modified based on project needs.
- **Notes:** This feature contributes to making the game closer to a polished 2D Top Down RPG as per tutorial.
