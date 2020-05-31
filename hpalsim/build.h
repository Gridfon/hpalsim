#include <string>
#include <vector>
using namespace std;

// === Fixed talents, essences, gear slots ===
// 1. Talents
// Crusader's Might: CS reduces HS cooldown by 1.5 sec
// Holy Avenger: 30% haste for 20 sec
// Sanctified Wrath: AW lasts +25% longer, and HS cooldown reduced by half
// Divine Purpose: HS has 20% chance to not trigger cooldown

// 2. Azerite pieces and traits
// Helm of Abyssal Malevolence: GoL+IJ+?+IV --- TODO: test OP vs OMW
// Lockjaw Shoulderplate: GoL+LD+EW+SP
// Deser Guardian's Breastplate: GoL+IJ+BS+VS

// 3. Azerite essences
// Strong DPS: Vision of Perfection / The Crucible of Flame / Conflict and Strife 
// Test these: Worldvein Resonance / Memory of Lucid Dreams / The Ever-Rising Tide / The Formless Void
// String healing: Spirit of Preservation


// Implement directly: 1 LD, 2 IJ, 3 GoL.
// Max corruption resistance: 125 + 10 + 39 @ https://worldofwarcraft.com/en-us/news/23227147#comments

enum class Corruptionw
{
    HASTE_PERC, CRIT_PERC, VERSA_PERC, CRIT_EFF, // 10, 15, ...
    HASTE_PROC, CRIT_PROC, VERSA_PROC, // 15, 20, ... [no 25]
    INEFFABLE_TRUTH, // 12 only
    GLIMPSE_OF_CLARITY, GUSHING_WOUND, // 15 only
    TWILIGHT_DEVASTATION, // 25, 50, ...
    VOID_RITUAL, TWISTED_APPENDAGE, // 15, 35, 66 - ?
    ECHOING_VOID, // 25, 35, 60 - ?
};

struct Equipment
{
    string name;
    int ILVL, INT, CRIT, HASTE, VERSA;
};

vector<Equipment> head{ Equipment{"Helm of Abyssal Malevolence", 480, 1514, 0, 0, 0} };
vector<Equipment> neck{ Equipment{"Heart of Azeroth", 513, 609, 392, 392, 392} };
vector<Equipment> shoulder{ Equipment{"Lockjaw Shoulderplate", 480, 1135, 0, 0, 0} };
vector<Equipment> back{ Equipment{"Ashjra'kamas, Shroud of Resolve", 500, 540, 72, 72, 0} };
vector<Equipment> chest{ Equipment{"Desert Guardian's Breastplate", 480, 1514, 0, 0, 0} };

vector<Equipment> wrist
{
    Equipment{"Dragonbone Vambraces",               475, 428,   0,  71, 117},
    Equipment{"Embalmer's Steadying Bracers",       465, 390, 111,  72,   0},
    Equipment{"Shadowshroud Vambraces",             465, 390, 123,   0,  60},
    Equipment{"Concealed Fencing Plates",           465, 390,  68,   0, 115},
};

vector<Equipment> hands
{
    Equipment{"Mirage-Weaver's Gauntlets",          470, 544, 115, 133,   0},
    Equipment{"Gauntlets of Total Subservience",    465, 519,   0, 143, 101},
    Equipment{"Sharktooth-Knuckled Grips",          465, 519,   0, 101, 143},
    Equipment{"Risen Lord's Oversized Gauntlets",   465, 519, 106,   0, 138},
};

vector<Equipment> waist
{
    Equipment{"Belt of the Unrelenting Gale",       475, 570, 110, 142,   0},
    Equipment{"Well-Oiled Plate Girdle",            475, 570, 123, 128,   0},
    Equipment{"Sharkbait Harness Girdle",           475, 570,   0, 174,  77},
    Equipment{"Malignant Leviathan's Clasp",        470, 544, 134,   0, 113},
};

vector<Equipment> legs
{
    Equipment{"Living Obsidian Legguards",          475, 760, 180, 155,   0},
    Equipment{"Legplates of Charged Duality",       465, 692, 121,   0, 205},
    Equipment{"Legplates of Beaten Gold",           465, 692,   0, 170, 156},
};

vector<Equipment> feet
{
    Equipment{"Sabatons of Rampaging Elements",     475, 570, 169,  82,   0},
    Equipment{"Ballast Sinkers",                    475, 570, 104, 147,   0},
    Equipment{"Pyroclastic Greatboots",             475, 570, 141,   0, 111},
    Equipment{"Fluorescent Flora Stompers",         465, 519,   0, 132, 111},
};

vector<Equipment> shield
{
    Equipment{"Bulwark of Brimming Potential",      475, 1166, 76,  0, 91},
    Equipment{"G0-4W4Y Crowd Repeller",             465, 1061, 67, 95,  0},
};

vector<Equipment> weapon
{
    Equipment{"Shgla'yos, Astral Malignity",        470, 2114, 52,  0,  0},
    Equipment{"Leaxa's Thought-Piercer",            465, 2015, 76, 86,  0},
    Equipment{"Sharpened Trogg Femur",              465, 2015, 80, 83,  0},
    Equipment{"Aq'mar, the Tidecaller",             465, 2015,  0, 64, 99},
};

vector<Equipment> finger
{
    Equipment{"Band of the Ancient Dredger",        475, 0, 311, 280,   0},
    Equipment{"Logic Loop of Synergy",              465, 0, 426, 147,   0},
    Equipment{"Logic Loop of Recursion",            465, 0, 426, 147,   0},
    Equipment{"Ring of Collective Consciousness",   465, 0, 322, 242,   0},
    Equipment{"Seal of Questionable Loyalties",     465, 0, 180, 393,   0},
    Equipment{"Ring of Cosmic Potential",           470, 0,   0, 233, 349},
    Equipment{"Ritual Binder's Ring",               465, 0,   0, 377, 197},
    Equipment{"Seal of the Regal Loa",              465, 0,   0, 223, 351},
    Equipment{"Thornwoven Band",                    465, 0,   0, 205, 369},
    Equipment{"Lord Waycrest's Signet",             465, 0, 369,   0, 205},
};

vector<Equipment> trinket
{
    Equipment{"Humming Black Dragonscale",          475, 723, 0, 0,   0},
    Equipment{"Forbidden Obsidian Claw",            475, 723, 0, 0,   0},
    Equipment{"Peerless Alchemist Stone",           470,   0, 0, 0, 236},
};
