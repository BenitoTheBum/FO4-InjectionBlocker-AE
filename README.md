**Leveled List Injection Blocker** by **GELUXRUM**, updated for the latest Fallout 4 Anniversary Edition (AE) runtime.

Built against CommonLibF4-NG with **Address Library** support—tested on the latest Fallout 4 AE update and backwards-compatible with Next-Gen (`1.10.984`) and Old-Gen (`1.10.163`) runtimes.

### What It Does
Blocks Papyrus startup quest scripts from dynamically injecting weapons, armor, ammo, or NPCs into vanilla leveled lists at runtime—preventing script-injected items from bypassing your leveled list patches or load order.

### Requirements
* **Fallout 4 Script Extender (F4SE)** (matching your Fallout 4 version)
* **Address Library for F4SE Plugins** (matching your Fallout 4 version)

### Configuration
Edit `Data\F4SE\Plugins\InjectionBlocker.ini` to toggle blocking per record category (`Ammo`, `Armor`, `Weapon`, `NPC`), or place whitelist `.ini` files in `Data\InjectionBlocker\` using `FormID~PluginName.esp=1` to allow specific scripted mods through.

### Credits & Source Code (GPLv3)
* **Original Mod:** [Leveled List Injection Blocker](https://nexusmods.com) by **GELUXRUM**
* **License:** Licensed under **GPLv3** in accordance with the original release.
* **Source Code:** [Your GitHub Repository Link Here]
