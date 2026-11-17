#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Item{
    string name;
    string type;
    int value;

    public:

    Item(string n, string t, int v) 
        :name(n), type(t), value(v){
    }

    void showItem() const{
        cout << "Name: " << name << "\n";
        cout << "Type: " << type << "\n";
        cout << "Value: " << value << "\n";
    }

    string getType() const{
        return type;
    }

    int getValue() const{
        return value;
    }

    string getName() const{
        return name;
    }    
};

class Character{
    string name;
    int health;
    int maxHealth;
    int level;
    vector<Item> inventory;
    
    inline static int pCount = 0;
    inline static int dCount = 0;

    public:

    Character(string n, int hp) :name(n), health(hp), maxHealth(hp), level(1){
        pCount++;
    }

    void showStats() const{

        if(health != 0){
            cout << name << " | HP: " << health << "/" << maxHealth << " | LEVEL: " << level << "\n";
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
            if(health>maxHealth){
                health = maxHealth;
            }
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

    void addItem(const Item& x){
        inventory.push_back(x);
        cout << x.getName() << " was added to " << name << "'s inventory.\n\n";  
        showInv();
    }

    void removeItem(int x, char r){
        if(x < 1 || static_cast<size_t>(x) > inventory.size()){
            cout << "Invalid item number.";
            return;
        }
        auto it = inventory.begin();
        it = it + x - 1;
        cout << (*it).getName() << " was removed from " << name << "'s inventory\n";
        if(r == 'd'){
            cout << "Reason: DROPPED.\n\n";
        }else if(r == 'c'){
            cout << "Reason: CONSUMED\n\n";
        }
        inventory.erase(it);
        showInv();
    }

    void showInv() const{
        cout << name << "'s Inventory:\n\n";
        if(inventory.empty()){
            cout << "This character has not collected any Items\n";
        }else{
            int count = 1;
            for(const Item& item: inventory){
                cout << count << ". \n";
                item.showItem();
                count++;
                cout << "\n";
            }
        }
    }

    //ITEM USAGE

    void dropItem(int x){
        //CHECK FOR CHARACTER STATUS
        if (health <= 0) {
            cout << "Character is dead.\n";
            return;
        }
        removeItem(x, 'd');
    }

    void useItem(int x){
        //CHECK FOR NO INVALID X
        if(x < 1 || static_cast<size_t>(x) > inventory.size()){
            cout << "Invalid item number.";
            return;
        }
        //CHECK FOR CHARACTER STATUS
        if (health <= 0) {
            cout << "Character is dead.\n";
            return;
        }
        
        if (inventory[x-1].getType() == "Potion") {
            if (inventory[x-1].getValue() <= 0) {
                cout << "Invalid potion value.\n";
                return;
            }
            if (health == maxHealth) {
                cout << "Idiot, you just wasted a potion while being at max HP.\n";
            }
            cout << inventory[x-1].getType() << " was used.\n";
            heal(inventory[x-1].getValue());
            cout << name << "'s HP: " << health << "/" << maxHealth << "\n";
            removeItem(x,'c');
        }
    }

    static void deathCount(){
        cout << "Characters dead: " << dCount << "\n";
    }
};

int main(){

    Character p1("Vortex", 100);
    Character p2("Siren", 120);

    Item i1("Iron Sword", "Weapon", 25);
    Item i2("Health Potion", "Potion", 50);

    p1.addItem(i1);
    p1.addItem(i2);

    p2.addItem(i1);
    p2.addItem(i2);

    p1.dmg(40);
    p1.useItem(2);

    p2.dropItem(1);

    Character::showCount();
    Character::deathCount();

}