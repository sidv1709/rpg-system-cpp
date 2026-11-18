#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Item{
    string name;
    string type;
    string statType;
    int value;
    string armorType;

    public:

    Item(string n, string t, string st, int v, string at = "") 
        :name(n), type(t), statType(st), value(v), armorType(at){
    }

    void showItem() const{
        cout << name << " | " << type << " | " << statType << ": " << value;
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

    string getArmorType() const{
        return armorType;
    }
};

class Character{
    struct ArmorSlot{
        string slotName;
        int itemIndex;
    };

    string name;
    int health;
    int maxHealth;
    int level;
    int equippedindex;
    vector<Item> inventory;
    vector<ArmorSlot> equippedArmor;
    
    inline static int pCount = 0;
    inline static int dCount = 0;

    public:

    Character(string n, int hp)
        :name(n), 
        health(hp), 
        maxHealth(hp), 
        level(1), 
        equippedindex(0),
        equippedArmor{
            {"Helmet", 0},
            {"ChestPlate", 0},
            {"Pants", 0},
            {"Shoes",0}
        }
    {
        pCount++;

        cout << "\n========== CHARACTER CREATED ==========\n\n";
        showStats();
        cout << "=======================================\n\n";
    }

    void showStats() const{

        if(health != 0){
            cout << name << " | HP: " << health << "/" << maxHealth << " | LEVEL: " << level << "\n\n";
        }else{
            cout << name << " is dead.\n\n";
        }
    }
    
    static void showCount() {
        cout << "Characters created: " << pCount << "\n\n";
    }

    void heal(int amt){
        if(amt <= 0){
            cout << "Doesnt heal using negative numbers because it will basically do damage idiot.\n\n";
            return;
        }
        if(health != 0){
            health += amt;
            if(health>maxHealth){
                health = maxHealth;
            }
        }else{
            cout << "This character cannot be healed since he/she has already died.\n\n";
            return;
        }
    }

    void dmg(int amt){
        if(amt < 0){
            cout << "Doesnt dmg using negative values because it will basically heal idiot.\n\n";
            return;
        }
        if(health == 0){
            cout << "This characters already dead!\n\n";
            return;
        }
        if(amt >= health){
            health = 0;
            cout << name << " has been killed.\n\n";
            dCount++;
            return;
        }else{
            health -= amt;
        }
        return;
    }

    void addItem(const Item& x){
        inventory.push_back(x);
        cout << x.getName() << " was added to " << name << "'s inventory.\n\n";  
        showInv();
    }

    void removeItem(int x, char r){
        if(x < 1 || static_cast<size_t>(x) > inventory.size()){
            cout << "Invalid item number.\n\n";
            return;
        }
        if(r != 'd' && r != 'c'){
            cout << "Invalid remove reason\n\n";
            return;
        }
        if(equippedindex == x){
            equippedindex = 0;
            cout << name << " unequipped " << inventory[x-1].getName() << ".\n\n";
        }
        else if(equippedindex > x){
            equippedindex--;
        }
        for(ArmorSlot& slot : equippedArmor){
            if(slot.itemIndex == x){
                slot.itemIndex = 0;
                cout << name << " unequipped " << inventory[x-1].getName() << ".\n\n";
            }else if(slot.itemIndex > x){
                slot.itemIndex--;
            }
        }
        auto it = inventory.begin();
        it = it + x - 1;
        if(r == 'd'){
            cout << name << " dropped " << (*it).getName() << ".\n\n";
        }
        inventory.erase(it);
        showInv();
    }

    void showInv() const{
        cout << name << "'s Inventory:\n";
        if(inventory.empty()){
            cout << "\nThis character has not collected any Items\n\n";
        }else{
            int count = 1;
            for(const Item& item: inventory){
                
                cout << count << ". ";
                item.showItem();
                if(item.getType() == "Weapon"){
                    if(count == equippedindex){
                        cout << " [EQUIPPED]";
                    }
                }else if(item.getType() == "Armor"){
                    for(const ArmorSlot& slot: equippedArmor){
                        if(count == slot.itemIndex){
                            cout << " [" << slot.slotName << "]";
                        }
                    }
                }
                count++;
                cout << "\n";
            }
            cout << "\n";
        }
    }

    //ITEM USAGE

    void dropItem(int x){
        //CHECK FOR CHARACTER STATUS
        if (health <= 0) {
            cout << "Character is dead.\n\n";
            return;
        }
        removeItem(x, 'd');
    }

    void useItem(int x){
        //CHECK FOR NO INVALID X
        if(x < 1 || static_cast<size_t>(x) > inventory.size()){
            cout << "Invalid item number.\n\n";
            return;
        }
        //CHECK FOR CHARACTER STATUS
        if (health <= 0) {
            cout << "Character is dead.\n\n";
            return;
        }
        
        if (inventory[x-1].getType() == "Potion") {
            if (inventory[x-1].getValue() <= 0) {
                cout << "Invalid potion value.\n\n";
                return;
            }
            if (health == maxHealth) {
                cout << "Idiot, you just wasted a potion while being at max HP.\n\n";
            }
            int hpres = min(inventory[x-1].getValue(), maxHealth - health);

            cout << name << " used " << inventory[x-1].getName() << ".\n";
            heal(inventory[x-1].getValue());
            cout << "HP Restored: " << hpres << "\n";
            cout << name << "'s HP: " << health << "/" << maxHealth << "\n\n";
            removeItem(x,'c');
        }
    }

    void equipItem(int x){
        if(x < 1 || static_cast<size_t>(x) > inventory.size()){
            cout << "Invalid Item Number.\n\n";
            return;
        }
        if(health <= 0){
            cout << "This charachter is dead how will he equip an item retard.\n\n";
            return;
        }
        if(inventory[x-1].getType() == "Potion"){
            cout << "Potions cannot be equipped.\n\n";
            return;
        }
        if(inventory[x-1].getType() == "Armor"){
            for(ArmorSlot& slot : equippedArmor){
                if(inventory[x-1].getArmorType() == slot.slotName){
                    if(slot.itemIndex == x){
                        cout << inventory[x-1].getName() << " is already equipped.\n\n";
                        return;
                    }else if(slot.itemIndex != 0){
                        cout << name << " unequipped " << inventory[slot.itemIndex - 1].getName() << ".\n";
                    }
                    cout << name << " equipped " << inventory[x-1].getName() << ".\n\n";
                    slot.itemIndex = x;
                    showInv();
                    return;              
                }
            }
            cout << "Invalid armor slot. Cannot equip this item.\n\n";
            return;
        }else if(inventory[x-1].getType() == "Weapon"){
            if(equippedindex == x){
                cout << inventory[x-1].getName() << " is already equipped.\n\n";
                return;
            }else if(equippedindex != 0){
                cout << name << " unequipped " << inventory[equippedindex - 1].getName() << ".\n";
            }
            cout << name << " equipped " << inventory[x-1].getName() << ".\n\n";
            equippedindex = x;
            showInv();
        }else{
            cout << "This isnt an equippable item.\n\n";
            return;
        }
        
    }

    void attack(Character& target){
        if(target.health == 0){
            cout << "Trying to attack a dead man, shame on you.\n\n";
            return;
        }
        if(health == 0){
            cout << "You are dead sit the fuck down.\n\n";
            return;
        }
        if(equippedindex == 0){
            cout << "Nothing's equipped you bot ass.\n\n";
            return;
        }
        if(inventory[equippedindex - 1].getType() != "Weapon"){
            cout << "The fuck are you gonna do with that throw it on him? Fucking chud.\n\n";
            return;
        }
        cout << name << " attacks " << target.name << "!\n";
        int finalDMG = target.calcDamage(inventory[equippedindex - 1].getValue());
        if(finalDMG!= 0){
            cout << "Damage dealt: " << finalDMG << "\n";
            target.dmg(finalDMG);
        }else{
            cout << target.name << "'s armour completely blocked " << name << "'s weak ass attack.\n\n";
        }
        
        if(target.health > 0){
            target.showStats();
        }
    }

    int getTotalDefence() const{
        int totalDefence = 0;
        for(const ArmorSlot& slot : equippedArmor){
            if(slot.itemIndex != 0){
                if(inventory[slot.itemIndex - 1].getValue() > 0){
                    totalDefence += inventory[slot.itemIndex - 1].getValue();
                }
            }
        }
        return totalDefence;
    }

    int calcDamage(int amt) const{
        int defence = getTotalDefence();
        int dmg = amt * (50.0 / (50.0 + defence));
        return dmg;
    }

    static void deathCount(){
        cout << "Characters dead: " << dCount << "\n\n";
    }
};


int main(){

    cout << "\n========== RPG SYSTEM CPP ==========\n";
    cout << "        CORE SYSTEM VERIFICATION\n";
    cout << "====================================\n\n";

    // ==========================================
    // COMBAT TESTS (1-5)
    // ==========================================

    Character p1("Vortex", 100);
    Character p2("Siren", 120);
    Character p3("Knight", 15);
    Character p4("Tank", 100);
    Character p5("Peasant", 100);

    Item sword("Iron Sword", "Weapon", "Damage", 25);
    Item helmet("Iron Helmet", "Armor", "Defence", 10, "Helmet");
    Item heavyHelmet("Heavy Helmet", "Armor", "Defence", 100, "Helmet");
    Item stick("Wooden Stick", "Weapon", "Damage", 1);

    cout << "\n===== TEST 1: NORMAL ATTACK =====\n\n";
    p1.addItem(helmet);
    p2.addItem(sword);
    p1.equipItem(1);
    p2.equipItem(1);
    p2.attack(p1);

    cout << "\n===== TEST 2: ATTACK WITHOUT WEAPON =====\n\n";
    p1.attack(p2);

    cout << "\n===== TEST 3: KILLING BLOW =====\n\n";
    p3.addItem(helmet);
    p3.equipItem(1);
    p2.attack(p3);
    Character::deathCount();

    cout << "\n===== TEST 4: ATTACK DEAD CHARACTER =====\n\n";
    p2.attack(p3);
    Character::deathCount();

    cout << "\n===== TEST 5: BLOCKED ATTACK =====\n\n";
    p4.addItem(heavyHelmet);
    p4.equipItem(1);
    p5.addItem(stick);
    p5.equipItem(1);
    p5.attack(p4);


    // ==========================================
    // EQUIPMENT TESTS (6-11)
    // ==========================================

    Character equipmentTester("Equipment Tester", 100);

    Item steelSword("Steel Sword", "Weapon", "Damage", 40);
    Item steelHelmet("Steel Helmet", "Armor", "Defence", 20, "Helmet");

    cout << "\n===== TEST 6: EQUIP WEAPON =====\n\n";
    equipmentTester.addItem(sword);
    equipmentTester.addItem(steelSword);
    equipmentTester.addItem(helmet);
    equipmentTester.addItem(steelHelmet);
    equipmentTester.equipItem(1);

    cout << "\n===== TEST 7: EQUIP SAME WEAPON =====\n\n";
    equipmentTester.equipItem(1);

    cout << "\n===== TEST 8: SWAP WEAPONS =====\n\n";
    equipmentTester.equipItem(2);

    cout << "\n===== TEST 9: SWAP ARMOUR =====\n\n";
    equipmentTester.equipItem(3);
    equipmentTester.equipItem(4);

    cout << "\n===== TEST 10: DROP EQUIPPED ITEM =====\n\n";
    equipmentTester.dropItem(2);

    cout << "\n===== TEST 11: INVENTORY INDEX SHIFT =====\n\n";
    equipmentTester.dropItem(1);
    equipmentTester.showInv();


    // ==========================================
    // POTION TESTS (12-15)
    // ==========================================

    Character potionTester1("Potion Tester 1", 100);
    Character potionTester2("Potion Tester 2", 100);
    Character potionTester3("Potion Tester 3", 100);

    Item potion("Health Potion", "Potion", "Heal", 50);

    potionTester1.addItem(potion);
    potionTester2.addItem(potion);
    potionTester3.addItem(potion);

    cout << "\n===== TEST 12: NORMAL HEALING =====\n\n";
    potionTester1.dmg(70);
    potionTester1.showStats();
    potionTester1.useItem(1);

    cout << "\n===== TEST 13: CAPPED HEALING =====\n\n";
    potionTester2.dmg(20);
    potionTester2.showStats();
    potionTester2.useItem(1);

    cout << "\n===== TEST 14: POTION AT FULL HP =====\n\n";
    potionTester3.useItem(1);

    cout << "\n===== TEST 15: POTION CONSUMPTION =====\n\n";
    potionTester1.showInv();
    potionTester2.showInv();
    potionTester3.showInv();


    // ==========================================
    // VALIDATION & COUNTER TESTS (16-21)
    // ==========================================

    Character validationTester("Validation Tester", 100);
    Character deathTester("Death Tester", 120);

    Item invalidArmor("Mystery Armor", "Armor", "Defence", 20, "Gloves");

    cout << "\n===== TEST 16: INVALID ITEM INDEX =====\n\n";
    validationTester.equipItem(999);

    cout << "\n===== TEST 17: INVALID ARMOUR SLOT =====\n\n";
    validationTester.addItem(invalidArmor);
    validationTester.equipItem(1);

    cout << "\n===== TEST 18: NEGATIVE DAMAGE =====\n\n";
    validationTester.dmg(-10);
    validationTester.showStats();

    cout << "\n===== TEST 19: ZERO DAMAGE =====\n\n";
    validationTester.dmg(0);
    validationTester.showStats();

    cout << "\n===== TEST 20: CHARACTER COUNT =====\n\n";
    Character::showCount();

    cout << "\n===== TEST 21: DEATH COUNT =====\n\n";
    deathTester.dmg(120);
    Character::deathCount();

    cout << "\n========== END OF TESTS ==========\n\n";

    return 0;
}
