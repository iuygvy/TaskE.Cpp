// Bartosz Bugajski
#include <iostream>
#include <string>

class PLAYER_CLASS
{
private:
    unsigned int max_health;
    unsigned int health;
    unsigned int atk;
    unsigned int agi;
    PLAYER_CLASS* next;
    PLAYER_CLASS* prev;

public:
    PLAYER_CLASS(unsigned int mh = 0, unsigned int h = 0, unsigned int a = 0, unsigned int ag = 0)
    : max_health(mh), health(h), atk(a), agi(ag), next(NULL), prev(NULL){}

    virtual unsigned int getRemainingHealth()
    {
        return 100 * health / max_health;
    }

    virtual unsigned int getDamage() =0;

    virtual unsigned int getAgility()
    {
        return agi;
    }

    virtual void takeDamage(unsigned int) =0;

    virtual void applyWinnerReward()
    {
        atk += 2;
        agi += 2;
    }

    virtual void cure()
    {
        health = max_health;
    }

    virtual void printParams() =0;

    virtual std::string getId()
    {
        return "";
    }

    friend class CAESAR_CLASS;
    friend class HUMAN_CLASS;
    friend class BEAST_CLASS;
    friend class SQUAD_CLASS;

private:
    virtual void die()
    {
        health = 0;
    }
};

class ARENA_CLASS;

class CAESAR_CLASS
{
public:
    unsigned int attacks;
    void judgeDeathOrLife(PLAYER_CLASS* player)
    {
        static unsigned int judged = 0;
        if(++judged % 3 == 0 && attacks % 2 == 0)
        {
            player->die();
        }
    }
};

class ARENA_CLASS
{
private:
    CAESAR_CLASS caesar;

public:
    unsigned int attacks = 0;

    ARENA_CLASS(CAESAR_CLASS* c)
    {
        caesar = *c;
    }

    void fight(PLAYER_CLASS* p1, PLAYER_CLASS* p2)
    {
        if(p1->getRemainingHealth() == 0 || p2->getRemainingHealth() == 0)
        {
            return;
        }
        if(p2->getAgility() > p1->getAgility())
        {
            PLAYER_CLASS* temp = p1;
            p1 = p2;
            p2 = temp;
        }

        p1->printParams();
        p2->printParams();
        attacks = 0;
        while(attacks < 40)
        {
            p2->takeDamage(p1->getDamage());
            attacks++;
            p2->printParams();
            if(p2->getRemainingHealth() < 10) break;

            p1->takeDamage(p2->getDamage());
            attacks++;
            p1->printParams();
            if(p1->getRemainingHealth() < 10) break;
        }

        caesar.attacks = attacks;
        if(p1->getRemainingHealth() > 0)
        {
            caesar.judgeDeathOrLife(p1);
            p1->printParams();
        }

        if(p2->getRemainingHealth() > 0)
        {
            caesar.judgeDeathOrLife(p2);
            p2->printParams();
        }

        if(p1->getRemainingHealth() > 0)
        {
            p1->applyWinnerReward();
            p1->cure();
        }

        if(p2->getRemainingHealth() > 0)
        {
            p2->applyWinnerReward();
            p2->cure();
        }

        p1->printParams();
        p2->printParams();
    }
};

class HUMAN_CLASS : virtual public PLAYER_CLASS
{
private:
    std::string id;
    unsigned int def;

public:
    HUMAN_CLASS(std::string s, unsigned int d = 10) : id(s), def(d), PLAYER_CLASS(200, 200, 30, 10){};

    unsigned int getDamage()
    {
        return atk;
    }

    void takeDamage(unsigned int dmg)
    {
        if(dmg > def + agi)
        {
            dmg -= def + agi;
            if(dmg < health)
            {
                health -= dmg;
            }
            else
            {
                die();
            }
        }
    }

    void printParams()
    {
        std::cout << id << ':';
        if(getRemainingHealth() > 0)
        {
            std::cout << max_health << ':' << health << ':' << getRemainingHealth() << "%:" <<
                      getDamage() << ':' << getAgility() << ':' << def << '\n';
        }
        else
        {
            std::cout << "R.I.P.\n";
        }
    }

    std::string getId()
    {
        return id;
    }
};

class BEAST_CLASS : virtual public PLAYER_CLASS
{
private:
    std::string id;

public:
    BEAST_CLASS(std::string s) : id(s), PLAYER_CLASS(150, 150, 40, 20){}

    unsigned int getDamage()
    {
        if(getRemainingHealth() < 25) return atk*2;
        return atk;
    }

    void takeDamage(unsigned int dmg)
    {
        if(dmg > agi/2)
        {
            dmg -= agi / 2;
            if(dmg < health)
            {
                health -= dmg;
            }
            else
            {
                die();
            }
        }
    }

    void printParams()
    {
        std::cout << id << ':';
        if(getRemainingHealth() > 0)
        {
            std::cout << max_health << ':' << health << ':' << getRemainingHealth() << "%:" <<
                      getDamage() << ':' << getAgility() << '\n';
        }
        else
        {
            std::cout << "R.I.P.\n";
        }
    }

    std::string getId()
    {
        return id;
    }
};

class BERSERKER_CLASS : public HUMAN_CLASS, public BEAST_CLASS
{
public:
    BERSERKER_CLASS(std::string s1, std::string s2)
    : HUMAN_CLASS(s1, 15), BEAST_CLASS(s2), PLAYER_CLASS(200, 200, 35, 5){}

    unsigned int getDamage()
    {
        if (getRemainingHealth() < 25 && getRemainingHealth() != 0)
        {
            return BEAST_CLASS::getDamage();
        }
        else
        {
            return HUMAN_CLASS::getDamage();
        }
    }

    void takeDamage(unsigned int dmg)
    {
        if (getRemainingHealth() < 25 && getRemainingHealth() != 0)
        {
            BEAST_CLASS::takeDamage(dmg);
        }
        else
        {
            HUMAN_CLASS::takeDamage(dmg);
        }
    }

    void printParams()
    {
        if (getRemainingHealth() < 25 && getRemainingHealth() != 0)
        {
            BEAST_CLASS::printParams();
        }
        else
        {
            HUMAN_CLASS::printParams();
        }
    }

    std::string getId()
    {
        if (getRemainingHealth() < 25 && getRemainingHealth() != 0)
        {
            return BEAST_CLASS::getId();
        }
        else
        {
            return HUMAN_CLASS::getId();
        }
    }
};

class SQUAD_CLASS : public PLAYER_CLASS
{
private:
    std::string id;
    PLAYER_CLASS* first;
    PLAYER_CLASS* last;
    unsigned int players;
public:
    SQUAD_CLASS(std::string s) : id(s), first(NULL), last(NULL), players(0), PLAYER_CLASS(0, 0, 0, 0){}

    void addPlayer(PLAYER_CLASS* player)
    {
        if(player->getRemainingHealth() == 0) return;
        player->next = NULL;
        player->prev = NULL;
        if(first == NULL)
        {
            first = player;
            last = player;
        }
        else
        {
            PLAYER_CLASS* temp = first;
            while(temp != NULL && temp->getId() <= player->getId())
            {
                if(temp->getId() == player->getId())
                {
                    if(temp->max_health < player->max_health) temp = temp->next;
                    else if(temp->max_health > player->max_health) break;
                    else if(temp->health < player->health) temp = temp->next;
                    else if(temp->health > player->health) break;
                    else if(temp->getRemainingHealth() < player->getRemainingHealth()) temp = temp->next;
                    else if(temp->getRemainingHealth() > player->getRemainingHealth()) break;
                    else if(temp->getDamage() < player->getDamage()) temp = temp->next;
                    else if(temp->getDamage() > player->getDamage()) break;
                    else if(temp->getAgility() < player->getAgility()) temp = temp->next;
                    else if(temp->getAgility() > player->getAgility()) break;
                    else break;
                }
                else
                {
                    temp = temp->next;
                }
            }
            if(temp == NULL)
            {
                last->next = player;
                last->next->prev = last;
                last = player;
            }
            else if(temp == first)
            {
                first->prev = player;
                first->prev->next = first;
                first = player;
            }
            else
            {
                temp->prev->next = player;
                player->prev = temp->prev;
                player->next = temp;
                temp->prev = player;
            }
        }
        players++;
    }

    void sort()
    {
        PLAYER_CLASS* temp = first;
        SQUAD_CLASS sq(id);
        while(temp != NULL)
        {
            PLAYER_CLASS* temp2 = temp;
            temp = temp->next;
            sq.addPlayer(temp2);
        }
        first = sq.first;
        last = sq.last;
    }

    void removePlayer(PLAYER_CLASS* player)
    {
        if(player->prev != NULL)
        {
            player->prev->next = player->next;
        }
        else
        {
            first = first->next;
            if(first != NULL) first->prev = NULL;
        }

        if(player->next != NULL)
        {
            player->next->prev = player->prev;
        }
        else
        {
            last = last->prev;
            if(last != NULL) last->next = NULL;
        }
        players--;
    }

    unsigned int getAgility()
    {
        PLAYER_CLASS* temp = first;
        unsigned int min = temp->getAgility();
        while (temp != NULL)
        {
            if(temp->getAgility() < min) min = temp->getAgility();
            temp = temp->next;
        }
        return min;
    }

    unsigned int getDamage()
    {
        PLAYER_CLASS* temp = first;
        unsigned int sum = 0;
        while (temp != NULL)
        {
            sum += temp->getDamage();
            temp = temp->next;
        }
        return sum;
    }

    void takeDamage(unsigned int dmg)
    {
        dmg = dmg / players;
        PLAYER_CLASS* temp = first;
        while(temp != NULL)
        {
            temp->takeDamage(dmg);
            if(temp->getRemainingHealth() == 0)
            {
                temp = temp->next;
                if(players == 1)
                {
                    first = NULL;
                    last = NULL;
                    players = 0;
                    break;
                }
                removePlayer(temp->prev);
            }
            else
            {
                temp = temp->next;
            }
        }
    }

    unsigned int getRemainingHealth()
    {
        PLAYER_CLASS* temp = first;
        if(temp == NULL) return 0;
        unsigned int max = temp->getRemainingHealth();
        while(temp != NULL)
        {
            if(temp->getRemainingHealth() > max)
            {
                max = temp->getRemainingHealth();
            }
            temp = temp->next;
        }
        return max;
    }

    void die()
    {
        PLAYER_CLASS* temp = first;
        while(temp != NULL)
        {
            temp->health = 0;
            temp = temp->next;
        }
        players = 0;
        first = NULL;
        last = NULL;
    }

    void applyWinnerReward()
    {
        PLAYER_CLASS* temp = first;
        while(temp != NULL)
        {
            temp->applyWinnerReward();
            temp = temp->next;
        }
    }

    void cure()
    {
        PLAYER_CLASS* temp = first;
        while (temp != NULL)
        {
            temp->cure();
            temp = temp->next;
        }
    }

    void printParams()
    {
        std::cout << id << ':';
        if(players != 0)
        {
             std::cout << players << ':' << getRemainingHealth() << "%:" << getDamage() << ':' << getAgility() << '\n';
             sort();
             PLAYER_CLASS* temp = first;
             while(temp != NULL)
             {
                 temp->printParams();
                 temp = temp->next;
             }
        }
        else
        {
            std::cout << "nemo\n";
        }
    }
};