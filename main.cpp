#include <iostream>
#include <string>
using namespace std;

class Character{
    string name;
    int health;
    int level;
    inline static int pCount = 0;
    inline static int dCount = 0;

    public:

    Character(string n, int hp) :name(n), health(hp), level(1){
        pCount++;
    }

    void showStats() const{

        if(health != 0){
            cout << "The characters name is " << name << " has health of " << health << " and is level " << level << "\n";
        }else{
            cout << name << " is dead.\n";
        }
    }
    
    static void showCount() {
        cout << "Characters created: " << pCount << "\n";
    }

    void heal(int amt){
        if(amt <= 0){
            cout << "Doesnt heal using negative numbers because it will basically do damage idiot.";
            return;
        }
        if(health != 0){
            health += amt;
        }else{
            cout << "This character cannot be healed since he/she has already died.";
        }
    }

    void dmg(int amt){
        if(amt <= 0){
            cout << "Doesnt dmg using negative values because it will basically heal idiot.";
            return;
        }
        if(health == 0){
            cout << "This characters already dead!\n";
            return;
        }
        if(amt >= health){
            health = 0;
            cout << name << " has been killed.\n";
            dCount++;
        }else{
            health -= amt;
        }
    }

    static void deathCount(){
        cout << "Characters dead: " << dCount << "\n";
    }
};

int main(){
    
    Character p1("Vortex", 100);
    Character p2("Siren", 120);

    p1.heal(50);
    p2.dmg(70);
    p1.dmg(170);

    p1.showStats();
    p2.showStats();
    Character::showCount();
    Character::deathCount();

}