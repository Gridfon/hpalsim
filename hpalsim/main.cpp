#include <iostream>
#include <queue>
#include <algorithm>
#include <map>
using namespace std;

class HolyPaladin;

typedef pair<double, void (HolyPaladin::*) ()> event_pair;
struct event_pair_comp
{
    bool operator()(event_pair const& lhs, event_pair const& rhs)
    {
        return lhs.first > rhs.first;
    }
};

enum class DynamicSpells { HOLY_SHOCK, CRUSADER_STRIKE, OTHER };
enum class EquipmentSlot { HEAD, NECK, SHOULDER, BACK, CHEST, WRIST, WEAPON, SHIELD, HANDS, WAIST, LEGS, FEET, FINGER, TRINKET};
const int SOCKETABLE_EQUIPMENT_SLOTS = 11;



/*
TODO:
- pre-pot
- how haste impacts cooldowns
- how GCD interacts with CS charges or HS procs?
*/

class HolyPaladin
{
private:
    double TIME;
    map<pair<int, int>, double> DAMAGE; // <crit, versa> ratings -> base damage done

    // Stat points for 1%
    const int CRIT_IN_ONE_PERCENT = 72;
    const int HASTE_IN_ONE_PERCENT = 68;
    const int VERSA_IN_ONE_PERCENT = 85;

    // Base stats
    const int BASE_INT = 1469;
    const int BASE_CRIT_RATING = 6 * CRIT_RATING; // 6% worth of stat points; is NOT affected by SEVERE corruption

    // Stats (secondaries - as rating points)
    int INT, CRIT_RATING, HASTE_RATING, VERSA_RATING;

    // Durations in seconds
    const int CONS_DURATION = 12;
    const int HA_DURATION = 20;
    const double AW_DURATION = (20.0 + 5.0) * 1.25; // extended by Light's Decree and Sanctified Wrath

    // Cooldowns in seconds
    const int HA_CD = 90;
    const int AW_CD = 90; // reduced by Vision of Perfection
    // The following are reduced by haste: X% haste -> cooldown is BASE_CD / ((100+X) / 100)
    const double BASE_GCD = 1.5;
    const int BASE_JUDGE_CD = 12;
    const int BASE_HS_CD = 9;
    const int BASE_CS_CD = 6; // 2 charges
    const double BASE_CONS_CD = 4.5;

    // Flat buffs 
    const int BUFF_FLASK = 360;
    const int BUFF_RUNE = 60;
    const int BUFF_FOOD = 93; // usually versatility

    // Multipliers
    const double BUFF_SCROLL = 1.07;
    const double BUFF_MAGE = 1.1;
    double INT_MULTIPLIER; // either mage buff or itellect scroll

    double BASE_DAMAGE_MULTIPLIER; // affected by Awenging Wrath
    double CRIT_DAMAGE_MULTIPLIER; // default value is 2.0; can be increased by Strikethrough corruption
    double HASTE_MULTIPLIER; // Haste: X% -> ((100+X) * HASTE_MULTIPLIER - 100)%
    double HS_CD_MULTIPLIER; // reduced by Awenging Wrath

    // Current state
    bool JUDGE_MULTIPLIER; // next CS or HS is affected by Judgment buff
    bool HA_ON_CD, AW_ON_CD, JUDGE_ON_CD, HS_ON_CD, CONS_ON_CD;
    bool GCD_WINDOW; // are we in GCD right now?
    int CS_CHARGES;

    // Event queues
    priority_queue<event_pair, vector<event_pair>, event_pair_comp> static_events; // time -> event
    //multimap<float, int> dynamic_cooldowns; // remaining cooldown -> event

    void FINISH_HA() { HASTE_MULTIPLIER = 1.0; }
    void FINISH_CS() { CS_CHARGES += 1; }

    void FINISH_AW()
    {
        BASE_DAMAGE_MULTIPLIER = 1.0;
        CRIT = CRIT - 30.0;
        HS_CD_MULTIPLIER = 1.0;
    }


    void process_CONS_tick()
    {
        // Base damage per tick: 0.109 * CURRENT_INT
    }

    void FINISH_CONS()
    {
        // TODO
    }

    void START_CONS()
    {
        // TODO
    }


    bool roll_for_crit(double crit_chance) // crit_chance in [0, 100]
    {
        // TODO
        return false;
    }

    void START_DYNAMIC_COOLDOWN(int dynamic_cooldown, void (HolyPaladin::* finish) ())
    {
        // TODO
    }


    void START_DYNAMIC_COOLDOWN(int dynamic_cooldown, bool *flag)
    {
        *flag = true;
        // TODO
    }

    void START_STATIC_COOLDOWN(int static_cooldown, bool* flag)
    {
        *flag = true;
        // TODO
    }

    void START_STATIC_PROC(void (HolyPaladin::* start) (), int duration, void (HolyPaladin::* finish) ())
    {
        static_events.push(make_pair(TIME + duration, finish));
    }

    void START_STATIC_PROC(int duration, void (HolyPaladin::* finish) ())
    {
        static_events.push(make_pair(TIME + duration, finish));
    }

    void REDUCE_HS_CD()
    {
        //TODO
    }

    void START_CONS_TICKS()
    {
    }

    void DEAL_DAMAGE(double damage_coefficient, DynamicSpells spell)
    {
        double crit_chance;
        if (spell == DynamicSpells::HOLY_SHOCK)
            crit_chance = CRIT + 30.0;
        else
            crit_chance = CRIT;
        bool crit_happens = roll_for_crit(crit_chance);

        double effective_int = INT * INT_MULTIPLIER;
        double base_damage = damage_coefficient * effective_int;
        double adjusted_damage = base_damage * BASE_DAMAGE_MULTIPLIER;
        if (spell == DynamicSpells::HOLY_SHOCK or spell == DynamicSpells::CRUSADER_STRIKE)
        {
            adjusted_damage = adjusted_damage * JUDGE_MULTIPLIER;
            JUDGE_MULTIPLIER = 1.0;
        }

        if (crit_happens)
        {
            double crit_damage = adjusted_damage * CRIT_DAMAGE_MULTIPLIER;
            DAMAGE = DAMAGE + crit_damage;
        }
        else
        {
            DAMAGE = DAMAGE + adjusted_damage;
        }
    }

public:
    HolyPaladin(int INT_ON_GEAR, int CRIT_ON_GEAR, int HASTE_ON_GEAR, int VERSA_ON_GEAR, bool MAGE_BUFF)
    {
        TIME = 0.0;
        DAMAGE = 0.0;

        INT = BASE_INT + INT_ON_GEAR + BUFF_FLASK + BUFF_RUNE;
        CRIT = BASE_CRIT + (CRIT_ON_GEAR / CRIT_RATING);
        HASTE = (HASTE_ON_GEAR / HASTE_RATING);
        VERSA = ((VERSA_ON_GEAR + BUFF_FOOD) / VERSA_RATING);

        if (MAGE_BUFF) INT_MULTIPLIER = BUFF_MAGE; else INT_MULTIPLIER = BUFF_SCROLL;
        JUDGE_MULTIPLIER = 1.0; // Judgement into CS or HS
        BASE_DAMAGE_MULTIPLIER = 1.0; // only changed by Avenging Wrath
        CRIT_DAMAGE_MULTIPLIER = 2.0; // only changed by Strikethrough corruption
        HASTE_MULTIPLIER = 1.0; // only changed by Holy Avenger
        HS_CD_MULTIPLIER = 1.0; // only changed by Avenging Wrath

        HA_ON_CD = AW_ON_CD = JUDGE_ON_CD = HS_ON_CD = CONS_ON_CD = false;
        CS_CHARGES = 2;

        GCD_WINDOW = false;
    }

    void HA()
    {
        if (GCD_WINDOW or HA_ON_CD) __assume(false);

        HASTE_MULTIPLIER = 1.3;
        START_STATIC_PROC(HA_DURATION, &HolyPaladin::FINISH_HA);

        START_STATIC_COOLDOWN(HA_CD, &HA_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };


    void AW() // with Light's Decree and Sanctified Wrath
    {
        if (GCD_WINDOW or AW_ON_CD) __assume(false);

        BASE_DAMAGE_MULTIPLIER = 1.3; // Increases all damage by 30%
        CRIT = CRIT + 30.0; // Increases crit chance by 30%
        HS_CD_MULTIPLIER = 0.5; // Reduces cooldown of HS by 2 due to Sanctified Wrath
        START_STATIC_PROC(AW_DURATION, &HolyPaladin::FINISH_AW);

        START_STATIC_COOLDOWN(AW_CD, &AW_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void JUDGE()
    {
        if (GCD_WINDOW or JUDGE_ON_CD) __assume(false);
        JUDGE_MULTIPLIER = 1.3; // Next CS or HS increased by 30%; we assume this debuff will be used before it expires
        DEAL_DAMAGE(1.125, DynamicSpells::OTHER);
        START_DYNAMIC_COOLDOWN(BASE_JUDGE_CD, &JUDGE_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void HS()
    {
        if (GCD_WINDOW or HS_ON_CD) __assume(false);
        DEAL_DAMAGE(0.77, DynamicSpells::HOLY_SHOCK);
        START_DYNAMIC_COOLDOWN(BASE_HS_CD * HS_CD_MULTIPLIER, &HS_ON_CD); // Cooldown affected by Sanctified Wrath
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void CS() // with Crusader's Might
    {
        if (GCD_WINDOW or CS_CHARGES == 0) __assume(false);

        DEAL_DAMAGE(0.5568, DynamicSpells::CRUSADER_STRIKE);
        // Base damage on Rezan and Stalker: 0.5568 * CURRENT_INT
        // Base damage on tooltip: 0.795 * CURRENT_INT

        if (HS_ON_CD) REDUCE_HS_CD(); // Reduces HS CD by 1.5 sec

        CS_CHARGES -= 1;
        START_DYNAMIC_COOLDOWN(BASE_CS_CD, &HolyPaladin::FINISH_CS);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void CONS()
    {
        if (GCD_WINDOW or CONS_ON_CD) __assume(false);

        START_CONS_TICKS(); // TICKING DAMAGE
        START_STATIC_PROC(&HolyPaladin::START_CONS, CONS_DURATION, &HolyPaladin::FINISH_CONS);

        START_DYNAMIC_COOLDOWN(BASE_CONS_CD, &CONS_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    // Trinkets and pots have no GCD
    void TRINKET_CLAW() // 2 min CD -- 20 sec on other trinket
    {
        // TODO
    }

    void TRINKET_MANIFESTO() // 1m 30 sec CD -- 20 sec on other trinket
    {
        // TODO
    }

    void POT_INT()
    {
        // 25 sec duration, 60 sec CD
        // TODO
    }

    void POT_UNBRIDLED()
    {
        // 1 min durationwa 
        // TODO
    }

    // TODO: bloodlust vs. drums
    // drums are 25% multiplicative haste for 40 sec
    // bloodlust is 30% 
    // TODO: white damage
    // TODO: assign damage to buckets, each for different crit rate

    // TODO: Glimmer of Lightx3
    // TODO: Indomitable Justicex2

    // TODO: Glimmer is affected by crit and versa, but not intellect
    // TODO: Judge+IJ affected by crit and versa, but not intellect

    // TODO: divine purpose: 20% holy shock repeated
};

int main()
{
    std::cout << "Hello World!\n";
}
