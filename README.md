# hi-octane-hook
A collection of code mods for the PC version of Cars: Mater-National, intended to be used alongside the Hi-Octane mod for extending the functionality of the game.
Currently planned features include:
- [x] UI elements now respond to 16x9 resolutions.
- [x] An interface for modifying game files at runtime was introduced through a file callback API. This feature is intended for developers.
- [x] The user can now store community/user-made patches in a mods/ folder located in the root of the game's installation.
- [x] Scenes can now have unique loading screens, as described in SceneInfo.dat.
- [x] WAV, MP3, and OGG are now all natively supported, instead of just OGG.
- [x] Characters that were previously unable to have a selected quote on the character selection screen now have selected quotes, a new file C\Global\Chars\DialogueList.ini is used to store a list of each of the added characters.
- [x] Monster trucks that previously used the normal character selection screen camera animation now use the monster truck-specific character selection screen camera animation. A new file C\Global\Chars\LargeVehicles.ini describes each character that should use this different animation.
- [x] A console window now relays information during runtime, used for many purposes, including showing every single file being loaded by the game to make debugging significantly easier.
- [x] The old .bin and .loc files are gone, and in their place are standard JSON files, in order to make creating and testing localization edits significantly easier.
- [X] The FE_Select_Continue and FE_Back sounds now play in the Main Menu, just as they do on the Xbox 360 and PS3 versions of the game.
