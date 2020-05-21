#include <iostream>
#include <queue>
#include <algorithm>
//#include <map>
using namespace std;

class HolyPaladin;
typedef pair<double, void (HolyPaladin::*) ()> event_pair;

enum dynamic_spells { HOLY_SHOCK, CRUSADER_STRIKE, OTHER };

struct event_pair_comp
{
    bool operator()(event_pair const& lhs, event_pair const& rhs)
    {
        return lhs.first > rhs.first;
    }
};

/*
TODO:
- pre-pot
- how haste impacts cooldowns
- how GCD interacts with CS charges or HS procs?
*/

class HolyPaladin
{
private:
    const int BASE_INT = 1469;
    const int BASE_CRIT = 6; // percent    

    // Stat points for 1%
    const double VERSA_RATING = 85.0;
    const double CRIT_RATING = 72.0;
    const double HASTE_RATING = 68.0;

    // Const seconds
    const int CONS_DURATION = 12;
    const int HA_DURATION = 20;
    const int HA_CD = 90;
    const double AW_DURATION = (20.0 + 5.0) * 1.25; // with Light's Decree and Sanctified Wrath
    const int AW_CD = 90; // with Vision of Perfection

    // Seconds, reduced by haste
    // X% haste -> cooldown is BASE_CD / ((100+X) / 100)
    const double BASE_GCD = 1.5;
    const int BASE_JUDGE_CD = 12;
    const int BASE_HS_CD = 9;
    const int BASE_CS_CD = 6; // 2 charges
    const double BASE_CONS_CD = 4.5;

    // Buffs
    const int BUFF_FOOD = 93;
    const int BUFF_FLASK = 360;
    const int BUFF_RUNE = 60;
    const double BUFF_SCROLL = 1.07;
    const double BUFF_MAGE = 1.1;
         
    // DAMAGE per TIME
    double TIME;
    int DAMAGE;

    // Stats, in percentages
    int INT;
    double CRIT, HASTE, VERSA;

    // Multipliers
    bool JUDGE_MULTIPLIER; // JUDGE affects next CS/HS
    double DEF_DAMAGE_MULTIPLIER; // Set by Awenging Wrath
    double CRIT_DAMAGE_MULTIPLIER; // Default value 2.0, changed by Strikethrough corruption
    double HASTE_MULTIPLIER; // Haste: X% -> ((100+X) * HASTE_MULTIPLIER - 100)%
    double INT_MULTIPLIER; // Mage buff or INT scroll
    double HS_CD_MULTIPLIER; // Set by Awenging Wrath

    // Ability cooldowns and charges
    bool HA_ON_CD, AW_ON_CD, JUDGE_ON_CD, HS_ON_CD, CS_ON_CD, CONS_ON_CD;
    int CS_CHARGES;

    // Are we in GCD right now?
    bool GCD_WINDOW;

    // Event queues
    priority_queue<event_pair, vector<event_pair>, event_pair_comp> static_events; // time -> event
    //multimap<float, int> dynamic_cooldowns; // remaining cooldown -> event

    void finish_HA()
    {
        HASTE_MULTIPLIER = 1.0;
    }


    void finish_AW()
    {
        DEF_DAMAGE_MULTIPLIER = 1.0;
        CRIT = CRIT - 30.0;
        HS_CD_MULTIPLIER = 1.0;
    }


    void process_CONS_tick()
    {
        // Base damage per tick: 0.109 * CURRENT_INT
    }

    void finish_CONS()
    {
        // TODO
    }

    bool roll_for_crit(double crit_chance) // crit_chance in [0, 100]
    {
        // TODO
        return false;
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

    void START_STATIC_PROC(int duration, void (HolyPaladin::* foo) ())
    {
        static_events.push(make_pair(TIME + duration, foo));
    }

    void REDUCE_HS_CD()
    {
        //TODO
    }

    void DEAL_DAMAGE(double damage_coefficient, dynamic_spells spell)
    {
        double crit_chance;
        if (spell == HOLY_SHOCK)
            crit_chance = CRIT + 30.0;
        else
            crit_chance = CRIT;
        bool crit_happens = roll_for_crit(crit_chance);

        double effective_int = INT * INT_MULTIPLIER;
        double base_damage = damage_coefficient * effective_int;
        double adjusted_damage = base_damage * DEF_DAMAGE_MULTIPLIER;
        if (spell == HOLY_SHOCK or spell == CRUSADER_STRIKE)
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
        DAMAGE = 0;
        JUDGE_MULTIPLIER = 1.0; 
        DEF_DAMAGE_MULTIPLIER = 1.0; // only changed by Avenging Wrath
        CRIT_DAMAGE_MULTIPLIER = 2.0; // only changed by Strikethrough corruption
        HASTE_MULTIPLIER = 1.0; // only changed by Holy Avenger
        HS_CD_MULTIPLIER = 1.0; // only changed by Avenging Wrath
        INT = BASE_INT + INT_ON_GEAR + BUFF_FLASK + BUFF_RUNE;
        if (MAGE_BUFF) INT_MULTIPLIER = BUFF_MAGE; else INT_MULTIPLIER = BUFF_SCROLL;
        CRIT = BASE_CRIT + (CRIT_ON_GEAR / CRIT_RATING);
        HASTE = (HASTE_ON_GEAR / HASTE_RATING);
        VERSA = ((VERSA_ON_GEAR + BUFF_FOOD) / VERSA_RATING);

        HA_ON_CD = AW_ON_CD = JUDGE_ON_CD = HS_ON_CD = CS_ON_CD = CONS_ON_CD = false;
        CS_CHARGES = 2;

        GCD_WINDOW = false;
    }

    void HA()
    {
        if (GCD_WINDOW or HA_ON_CD) __assume(false);

        HASTE_MULTIPLIER = 1.3;
        START_STATIC_PROC(HA_DURATION, &HolyPaladin::finish_HA);

        START_STATIC_COOLDOWN(HA_CD, &HA_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };


    void AW() // with Light's Decree and Sanctified Wrath
    {
        if (GCD_WINDOW or AW_ON_CD) __assume(false);

        DEF_DAMAGE_MULTIPLIER = 1.3; // Increases all damage by 30%
        CRIT = CRIT + 30.0; // Increases crit chance by 30%
        HS_CD_MULTIPLIER = 0.5; // Reduces cooldown of HS by 2 due to Sanctified Wrath
        START_STATIC_PROC(AW_DURATION, &HolyPaladin::finish_AW);

        START_STATIC_COOLDOWN(AW_CD, &AW_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void JUDGE()
    {
        if (GCD_WINDOW or JUDGE_ON_CD) __assume(false);
        JUDGE_MULTIPLIER = 1.3; // Next CS or HS increased by 30%; we assume this debuff will be used before it expires
        DEAL_DAMAGE(1.125, OTHER);
        START_DYNAMIC_COOLDOWN(BASE_JUDGE_CD, &JUDGE_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void HS()
    {
        if (GCD_WINDOW or HS_ON_CD) __assume(false);
        DEAL_DAMAGE(0.77, HOLY_SHOCK);
        START_DYNAMIC_COOLDOWN(BASE_HS_CD * HS_CD_MULTIPLIER, &HS_ON_CD); // Cooldown affected by Sanctified Wrath
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void CS() // with Crusader's Might
    {
        if (GCD_WINDOW or CS_ON_CD) __assume(false);

        DEAL_DAMAGE(0.5568, CRUSADER_STRIKE);
        // Base damage on Rezan: 0.5568 * CURRENT_INT
        // (Base damage on tooltip: 0.795 * CURRENT_INT)

        if (HS_ON_CD) REDUCE_HS_CD(); // Reduces HS CD by 1.5 sec

        START_DYNAMIC_COOLDOWN(BASE_CS_CD, &CS_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };

    void CONS()
    {
        if (GCD_WINDOW or CONS_ON_CD) __assume(false);

        // TODO: DEAL TICK DAMGE

        ADD_STATIC_EVENT(CONS_DURATION, &HolyPaladin::finish_CONS);
        // TODO: handle reniewing this

        START_DYNAMIC_COOLDOWN(BASE_CONS_CD, &CONS_ON_CD);
        START_DYNAMIC_COOLDOWN(BASE_GCD, &GCD_WINDOW);
    };
};

int main()
{
    std::cout << "Hello World!\n";
}
