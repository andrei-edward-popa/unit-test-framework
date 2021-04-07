#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdint>

class Test {
public:
	int tmp;
	float ftmp;
	void print() {
		std::cout << "Inside method 'print' from class 'Test'\n";
	}
	int sum(int a, int b) {
		return a + b + tmp;
	}
	Test() : tmp(10), ftmp(12.45f) { };
	Test(int a, float b) : tmp(a), ftmp(b) { }
	Test operator+(const Test& test) {
		return Test(tmp + test.tmp, ftmp + test.ftmp);
	}
};

class Person {
public:
    std::string mName;
    uint8_t mAge;
    double mSalary;
    Person() : mAge(-1), mSalary(-1) {}
    Person(std::string name, uint8_t age, double salary) : mName(std::move(name)), mAge(age), mSalary(salary) {}
    bool operator==(const Person& other) const { return mName == other.mName && mAge == other.mAge && mSalary == other.mSalary; }
    bool operator<(const Person& other) const { return mName < other.mName; }
    virtual ~Person() = default;
};

class Building {
protected:
    std::string mBuildingName;
    std::string mType;
    Person mCoordinator;
    std::vector<Person> mEmployees;
    std::vector<Person> mCitizens;
public:
	Building() = default;
    Building(std::string type, std::string buildingName) : mType(std::move(type)), mBuildingName(std::move(buildingName)) {}
    virtual void add(const Person& person) = 0;
    virtual void remove(const Person& person) = 0;
    virtual Building* operator+(const Person& person) = 0;
    virtual Building* operator-() { return nullptr; }
    virtual Building* operator-(const Person& person) { return nullptr; }
    [[nodiscard]] Person getCoordinator() const { return mCoordinator; }
    [[nodiscard]] uint16_t getCitizensSize() const { return mCitizens.size(); }
    [[nodiscard]] uint16_t getEmployeesSize() const { return mEmployees.size(); }
    [[nodiscard]] Person getEmployee(uint16_t index) const { return mEmployees[index]; }
    [[nodiscard]] Person getCitizen(uint16_t index) const { return mCitizens[index]; }
    [[nodiscard]] std::string getBuildingName() const { return mBuildingName; }
    [[nodiscard]] std::string getType() const { return mType; }
    virtual ~Building() = default;
};

class CityHall : public Building {
public:
    explicit CityHall(std::string buildingName) : Building("CityHall", std::move(buildingName)) {
        mEmployees.clear();
        mCoordinator = Person();
    }
    Building* operator+(const Person& mayor) override {
        if (mCoordinator == Person()) { mCoordinator = mayor; }
        return this;
    }
    Building* operator-() override {
        mCoordinator = Person();
        return this;
    }
    void add(const Person& employee) override { mEmployees.emplace_back(employee); }
    void remove(const Person& employee) override {
        mEmployees.erase(std::find(mEmployees.begin(), mEmployees.end(), employee));
    }
    virtual ~CityHall() = default;
};

class Hospital : public Building {
public:
    explicit Hospital(std::string buildingName) : Building("Hospital", std::move(buildingName)) { mCitizens.clear(); mEmployees.clear(); }
    Building* operator+(const Person& doctor) override {
        mEmployees.emplace_back(doctor);
        return this;
    }
    Building* operator-(const Person& doctor) override {
        mEmployees.erase(std::find(mEmployees.begin(), mEmployees.end(), doctor));
        return this;
    }
    void add(const Person& patient) override { mCitizens.emplace_back(patient); }
    void remove(const Person& patient) override {
        mCitizens.erase(std::find(mCitizens.begin(), mCitizens.end(), patient));
    }
    virtual ~Hospital() = default;
};

class PoliceStation : public Building {
public:
    explicit PoliceStation(std::string buildingName) : Building("PoliceStation", std::move(buildingName)) { mCitizens.clear(); mEmployees.clear(); }
    Building* operator+(const Person& policeMan) override {
        mEmployees.emplace_back(policeMan);
        return this;
    }
    Building* operator-(const Person& policeMan) override {
        mEmployees.erase(std::find(mEmployees.begin(), mEmployees.end(), policeMan));
        return this;
    }
    void add(const Person& busted) override { mCitizens.emplace_back(busted); }
    void remove(const Person& busted) override {
        mCitizens.erase(std::find(mCitizens.begin(), mCitizens.end(), busted));
    }
    virtual ~PoliceStation() = default;
};

class House : public Building {
    uint16_t mMaxNumber;
public:
    House(std::string buildingName, uint16_t maxNumber) : Building("House", std::move(buildingName)) {
        mCoordinator = Person();
        mCitizens.clear();
        mMaxNumber = maxNumber;
    }
    Building* operator+(const Person& owner) override {
        if (mCoordinator == Person()) { mCoordinator = owner; }
        return this;
    }
    Building* operator-() override {
        mCoordinator = Person();
        return this;
    }
    void add(const Person& citizen) override { if (mCitizens.size() + 1 < mMaxNumber) mCitizens.emplace_back(citizen); }
    void remove(const Person& citizen) override {
        if (citizen.mAge >= 18) {
            mCitizens.erase(std::find(mCitizens.begin(), mCitizens.end(), citizen));
        }
    }
    virtual ~House() = default;
};

class Block : public Building {
    uint16_t mMaxNumberPerFloor;
public:
    Block(std::string buildingName, uint16_t floors, uint16_t maxNumberPerFloor) : Building("Block", std::move(buildingName)) {
        mCoordinator = Person();
        mCitizens.clear();
        mMaxNumberPerFloor = maxNumberPerFloor;
        mCitizens = std::vector<Person>(mMaxNumberPerFloor * floors, Person());
    }
    Building* operator+(const Person& owner) override {
        if (mCoordinator == Person()) { mCoordinator = owner; }
        return this;
    }
    Building* operator-() override {
        mCoordinator = Person();
        return this;
    }
    void add(const Person& citizen) override {
        for (int i = 0; i < mCitizens.size(); i++) {
            if (citizen == Person()) {
                mCitizens.emplace_back(citizen);
                break;
            }
        }
    }
    void remove(const Person& citizen) override {
        if (citizen.mAge >= 18) {
            *std::find(mCitizens.begin(), mCitizens.end(), citizen) = Person();
        }
    }
    virtual ~Block() = default;
};

Person newPerson(const int a, int b, Block& p, const double* d) {
	return Person();
}

int multiply(int a, int b) {
	return a * b;
}

int main() {
    
    int a;
    std::cin >> a;
    std::cout << a;
    
    for (uint64_t i = 0; i < 1000000000; i++) {
	
	}
    
    return 0;
}
