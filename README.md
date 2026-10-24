# libobigplanet

C++ library for reading/writing and manipulating files from the LittleBigPlanet games.

Fully supported formats:

* FART (.farc, big/littlefart)
* MAP (.map)
* Translation Table (.trans)

In-progress/planned formats:

* Popit Item (.plan)
* Level
* Level Slots (.slt)
* Material (.mat)
* GMAT (.gmat)
* Bevel (.bev)
* Font (.fnt)
* Text-based formats (.mus, .joint, .gsub, etc)

Requires nlohmann-json, OpenSSL, utf8cpp, and zlib.

Credit to ennuo's [cwlib](https://github.com/ennuo/toolkit/tree/main/lib/cwlib) for reference.
