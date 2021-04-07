/**
  * Unit Test Framework for compile time checking in VPL
  * @Copyright (C) 2020 Andrei-Edward Popa
  * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
  */

REGISTER_CLASS(Block) {
    REGISTER_ATTRIBUTE(mMaxNumberPerFloor);
    REGISTER_METHOD(add);
    REGISTER_METHOD(remove);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_OPERATOR(operator-, operatorMinus);
    REGISTER_ATTRIBUTE_SIGNATURE(mMaxNumberPerFloor, uint16_t, ugwwvsyb);
    REGISTER_METHOD_SIGNATURE(add, void(Class::*)(const Person &) , vigmjjre);
    REGISTER_METHOD_SIGNATURE(remove, void(Class::*)(const Person &) , oeeynvio);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Building*(Class::*)(const Person &) , vrocytuc);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)() , czidquey);
};

REGISTER_CLASS(Building) {
    REGISTER_ATTRIBUTE(mEmployees);
    REGISTER_ATTRIBUTE(mCoordinator);
    REGISTER_ATTRIBUTE(mCitizens);
    REGISTER_ATTRIBUTE(mBuildingName);
    REGISTER_ATTRIBUTE(mType);
    REGISTER_METHOD(getBuildingName);
    REGISTER_METHOD(getType);
    REGISTER_METHOD(getCitizensSize);
    REGISTER_METHOD(remove);
    REGISTER_METHOD(getEmployee);
    REGISTER_METHOD(getCitizen);
    REGISTER_METHOD(getEmployeesSize);
    REGISTER_METHOD(getCoordinator);
    REGISTER_METHOD(add);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_OPERATOR(operator-, operatorMinus);
    REGISTER_ATTRIBUTE_SIGNATURE(mBuildingName, std::string, cdjgpzql);
    REGISTER_ATTRIBUTE_SIGNATURE(mCitizens, std::vector<Person>, qvcgnwuz);
    REGISTER_ATTRIBUTE_SIGNATURE(mCoordinator, Person, cvwbaedr);
    REGISTER_ATTRIBUTE_SIGNATURE(mEmployees, std::vector<Person>, snxzknng);
    REGISTER_ATTRIBUTE_SIGNATURE(mType, std::string, xkuvmglo);
    REGISTER_METHOD_SIGNATURE(add, void(Class::*)(const Person &) , mgdwnzsj);
    REGISTER_METHOD_SIGNATURE(getBuildingName, std::string(Class::*)() const, qyqufkzo);
    REGISTER_METHOD_SIGNATURE(getCitizen, Person(Class::*)(uint16_t) const, idnndqdd);
    REGISTER_METHOD_SIGNATURE(getCitizensSize, uint16_t(Class::*)() const, zsrnaffr);
    REGISTER_METHOD_SIGNATURE(getCoordinator, Person(Class::*)() const, ctjisbzd);
    REGISTER_METHOD_SIGNATURE(getEmployee, Person(Class::*)(uint16_t) const, xdqgiomk);
    REGISTER_METHOD_SIGNATURE(getEmployeesSize, uint16_t(Class::*)() const, iskworls);
    REGISTER_METHOD_SIGNATURE(getType, std::string(Class::*)() const, xfnnjivx);
    REGISTER_METHOD_SIGNATURE(remove, void(Class::*)(const Person &) , eqvwuojb);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Building*(Class::*)(const Person &) , fxblakqi);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)() , ofunvjdh);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)(const Person &) , cypmhicg);
};

REGISTER_CLASS(CityHall) {
    REGISTER_METHOD(add);
    REGISTER_METHOD(remove);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_OPERATOR(operator-, operatorMinus);
    REGISTER_METHOD_SIGNATURE(add, void(Class::*)(const Person &) , npvinakx);
    REGISTER_METHOD_SIGNATURE(remove, void(Class::*)(const Person &) , nojuxurg);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Building*(Class::*)(const Person &) , fakzcxhp);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)() , gbwjlndw);
};

REGISTER_CLASS(Hospital) {
    REGISTER_METHOD(add);
    REGISTER_METHOD(remove);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_OPERATOR(operator-, operatorMinus);
    REGISTER_METHOD_SIGNATURE(add, void(Class::*)(const Person &) , kipduwru);
    REGISTER_METHOD_SIGNATURE(remove, void(Class::*)(const Person &) , beyanvbm);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Building*(Class::*)(const Person &) , slzytbms);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)(const Person &) , zgoyqlmc);
};

REGISTER_CLASS(House) {
    REGISTER_ATTRIBUTE(mMaxNumber);
    REGISTER_METHOD(add);
    REGISTER_METHOD(remove);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_OPERATOR(operator-, operatorMinus);
    REGISTER_ATTRIBUTE_SIGNATURE(mMaxNumber, uint16_t, zvwuommn);
    REGISTER_METHOD_SIGNATURE(add, void(Class::*)(const Person &) , pihdhpgp);
    REGISTER_METHOD_SIGNATURE(remove, void(Class::*)(const Person &) , eeybzznm);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Building*(Class::*)(const Person &) , nzkfiyod);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)() , qihxinex);
};

REGISTER_CLASS(Person) {
    REGISTER_ATTRIBUTE(mAge);
    REGISTER_ATTRIBUTE(mName);
    REGISTER_ATTRIBUTE(mSalary);
    REGISTER_OPERATOR(operator==, operatorEqual);
    REGISTER_OPERATOR(operator<, operatorLess);
    REGISTER_ATTRIBUTE_SIGNATURE(mAge, uint8_t, cjwmdrxm);
    REGISTER_ATTRIBUTE_SIGNATURE(mName, std::string, agcadypv);
    REGISTER_ATTRIBUTE_SIGNATURE(mSalary, double, ferjsjpd);
    REGISTER_OPERATOR_SIGNATURE(operator<, operatorLess, bool(Class::*)(const Person &) const, kdfngvaz);
    REGISTER_OPERATOR_SIGNATURE(operator==, operatorEqual, bool(Class::*)(const Person &) const, fqcdvuib);
};

REGISTER_CLASS(PoliceStation) {
    REGISTER_METHOD(add);
    REGISTER_METHOD(remove);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_OPERATOR(operator-, operatorMinus);
    REGISTER_METHOD_SIGNATURE(add, void(Class::*)(const Person &) , ydejvdbh);
    REGISTER_METHOD_SIGNATURE(remove, void(Class::*)(const Person &) , pqwuccaa);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Building*(Class::*)(const Person &) , ayijppwo);
    REGISTER_OPERATOR_SIGNATURE(operator-, operatorMinus, Building*(Class::*)(const Person &) , glejjnse);
};

REGISTER_CLASS(Test) {
    REGISTER_ATTRIBUTE(ftmp);
    REGISTER_ATTRIBUTE(tmp);
    REGISTER_METHOD(sum);
    REGISTER_METHOD(print);
    REGISTER_OPERATOR(operator+, operatorPlus);
    REGISTER_ATTRIBUTE_SIGNATURE(ftmp, float, umerrmkx);
    REGISTER_ATTRIBUTE_SIGNATURE(tmp, int, rsutbuew);
    REGISTER_METHOD_SIGNATURE(print, void(Class::*)() , aqxcsumc);
    REGISTER_METHOD_SIGNATURE(sum, int(Class::*)(int , int) , oexhjhap);
    REGISTER_OPERATOR_SIGNATURE(operator+, operatorPlus, Test(Class::*)(const Test &) , crdnycgl);
};

REGISTER_FUNCTION(newPerson);
REGISTER_FUNCTION_SIGNATURE(newPerson, GlobalFunctionsWrapper::Person(Function::*)(const int , int , GlobalFunctionsWrapper::Block &, const double *), kuttypvc);
REGISTER_FUNCTION(multiply);
REGISTER_FUNCTION_SIGNATURE(multiply, int(Function::*)(int , int), bdiygpvz);

TEST(Segmentation, Fault) {
    /* insert code */
}

TEST(Function, multiply) {

    ASSERT_FUNCTION(multiply, "Funcția 'multiply' nu există!");
    ASSERT_FUNCTION_SIGNATURE(multiply, bdiygpvz, "Funcția 'multiply' nu conține tipul specificat în cerință!") BEGIN {
        /* insert code */
    } END

}

TEST(Function, newPerson) {

    ASSERT_FUNCTION(newPerson, "Funcția 'newPerson' nu există!");
    ASSERT_FUNCTION_SIGNATURE(newPerson, kuttypvc, "Funcția 'newPerson' nu conține tipul specificat în cerință!") BEGIN {
        /* insert code */
    } END

}

TEST(Existence, Block) {

    ASSERT_CLASS(Block, "Clasa 'Block' nu există!");

}

TEST(Existence, Building) {

    ASSERT_CLASS(Building, "Clasa 'Building' nu există!");

}

TEST(Existence, CityHall) {

    ASSERT_CLASS(CityHall, "Clasa 'CityHall' nu există!");

}

TEST(Existence, Hospital) {

    ASSERT_CLASS(Hospital, "Clasa 'Hospital' nu există!");

}

TEST(Existence, House) {

    ASSERT_CLASS(House, "Clasa 'House' nu există!");

}

TEST(Existence, Person) {

    ASSERT_CLASS(Person, "Clasa 'Person' nu există!");

}

TEST(Existence, PoliceStation) {

    ASSERT_CLASS(PoliceStation, "Clasa 'PoliceStation' nu există!");

}

TEST(Existence, Test) {

    ASSERT_CLASS(Test, "Clasa 'Test' nu există!");

}

TEST(Destructor, Block) {

    ASSERT_DESTRUCTOR("Clasa 'Block' nu conține un destructor!", Block);

}

TEST(Destructor, Building) {

    ASSERT_DESTRUCTOR("Clasa 'Building' nu conține un destructor!", Building);

}

TEST(Destructor, CityHall) {

    ASSERT_DESTRUCTOR("Clasa 'CityHall' nu conține un destructor!", CityHall);

}

TEST(Destructor, Hospital) {

    ASSERT_DESTRUCTOR("Clasa 'Hospital' nu conține un destructor!", Hospital);

}

TEST(Destructor, House) {

    ASSERT_DESTRUCTOR("Clasa 'House' nu conține un destructor!", House);

}

TEST(Destructor, Person) {

    ASSERT_DESTRUCTOR("Clasa 'Person' nu conține un destructor!", Person);

}

TEST(Destructor, PoliceStation) {

    ASSERT_DESTRUCTOR("Clasa 'PoliceStation' nu conține un destructor!", PoliceStation);

}

TEST(Abstract, Building) {

    ASSERT_ABSTRACT(Building, "Clasa 'Building' nu este abstractă!");

}

TEST(Inheritance, Block) {

    ASSERT_INHERITANCE(Building, Block, "Clasa 'Block' nu este moștenită din clasa 'Building'!");

}

TEST(Inheritance, CityHall) {

    ASSERT_INHERITANCE(Building, CityHall, "Clasa 'CityHall' nu este moștenită din clasa 'Building'!");

}

TEST(Inheritance, Hospital) {

    ASSERT_INHERITANCE(Building, Hospital, "Clasa 'Hospital' nu este moștenită din clasa 'Building'!");

}

TEST(Inheritance, House) {

    ASSERT_INHERITANCE(Building, House, "Clasa 'House' nu este moștenită din clasa 'Building'!");

}

TEST(Inheritance, PoliceStation) {

    ASSERT_INHERITANCE(Building, PoliceStation, "Clasa 'PoliceStation' nu este moștenită din clasa 'Building'!");

}

TEST(BlockAttribute, mMaxNumberPerFloor) {

    ASSERT_ATTRIBUTE(Block, mMaxNumberPerFloor, "Clasa 'Block' nu conține atributul 'mMaxNumberPerFloor'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Block, mMaxNumberPerFloor, ugwwvsyb, "Clasa 'Block' nu conține atributul 'mMaxNumberPerFloor' cu tipul specificat în cerință!");

}

TEST(BuildingAttribute, mBuildingName) {

    ASSERT_ATTRIBUTE(Building, mBuildingName, "Clasa 'Building' nu conține atributul 'mBuildingName'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Building, mBuildingName, cdjgpzql, "Clasa 'Building' nu conține atributul 'mBuildingName' cu tipul specificat în cerință!");

}

TEST(BuildingAttribute, mCitizens) {

    ASSERT_ATTRIBUTE(Building, mCitizens, "Clasa 'Building' nu conține atributul 'mCitizens'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Building, mCitizens, qvcgnwuz, "Clasa 'Building' nu conține atributul 'mCitizens' cu tipul specificat în cerință!");

}

TEST(BuildingAttribute, mCoordinator) {

    ASSERT_ATTRIBUTE(Building, mCoordinator, "Clasa 'Building' nu conține atributul 'mCoordinator'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Building, mCoordinator, cvwbaedr, "Clasa 'Building' nu conține atributul 'mCoordinator' cu tipul specificat în cerință!");

}

TEST(BuildingAttribute, mEmployees) {

    ASSERT_ATTRIBUTE(Building, mEmployees, "Clasa 'Building' nu conține atributul 'mEmployees'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Building, mEmployees, snxzknng, "Clasa 'Building' nu conține atributul 'mEmployees' cu tipul specificat în cerință!");

}

TEST(BuildingAttribute, mType) {

    ASSERT_ATTRIBUTE(Building, mType, "Clasa 'Building' nu conține atributul 'mType'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Building, mType, xkuvmglo, "Clasa 'Building' nu conține atributul 'mType' cu tipul specificat în cerință!");

}

TEST(HouseAttribute, mMaxNumber) {

    ASSERT_ATTRIBUTE(House, mMaxNumber, "Clasa 'House' nu conține atributul 'mMaxNumber'!");
    ASSERT_ATTRIBUTE_SIGNATURE(House, mMaxNumber, zvwuommn, "Clasa 'House' nu conține atributul 'mMaxNumber' cu tipul specificat în cerință!");

}

TEST(PersonAttribute, mAge) {

    ASSERT_ATTRIBUTE(Person, mAge, "Clasa 'Person' nu conține atributul 'mAge'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Person, mAge, cjwmdrxm, "Clasa 'Person' nu conține atributul 'mAge' cu tipul specificat în cerință!");

}

TEST(PersonAttribute, mName) {

    ASSERT_ATTRIBUTE(Person, mName, "Clasa 'Person' nu conține atributul 'mName'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Person, mName, agcadypv, "Clasa 'Person' nu conține atributul 'mName' cu tipul specificat în cerință!");

}

TEST(PersonAttribute, mSalary) {

    ASSERT_ATTRIBUTE(Person, mSalary, "Clasa 'Person' nu conține atributul 'mSalary'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Person, mSalary, ferjsjpd, "Clasa 'Person' nu conține atributul 'mSalary' cu tipul specificat în cerință!");

}

TEST(TestAttribute, ftmp) {

    ASSERT_ATTRIBUTE(Test, ftmp, "Clasa 'Test' nu conține atributul 'ftmp'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Test, ftmp, umerrmkx, "Clasa 'Test' nu conține atributul 'ftmp' cu tipul specificat în cerință!");

}

TEST(TestAttribute, tmp) {

    ASSERT_ATTRIBUTE(Test, tmp, "Clasa 'Test' nu conține atributul 'tmp'!");
    ASSERT_ATTRIBUTE_SIGNATURE(Test, tmp, rsutbuew, "Clasa 'Test' nu conține atributul 'tmp' cu tipul specificat în cerință!");

}

TEST(BlockMethod, add) {

    ASSERT_METHOD(Block, add, "Clasa 'Block' nu conține metoda 'add'!");
    ASSERT_METHOD_SIGNATURE(Block, add, vigmjjre, "Clasa 'Block' nu conține metoda 'add' cu tipul specificat în cerință!");

}

TEST(BlockMethod, remove) {

    ASSERT_METHOD(Block, remove, "Clasa 'Block' nu conține metoda 'remove'!");
    ASSERT_METHOD_SIGNATURE(Block, remove, oeeynvio, "Clasa 'Block' nu conține metoda 'remove' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, add) {

    ASSERT_METHOD(Building, add, "Clasa 'Building' nu conține metoda 'add'!");
    ASSERT_METHOD_SIGNATURE(Building, add, mgdwnzsj, "Clasa 'Building' nu conține metoda 'add' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getBuildingName) {

    ASSERT_METHOD(Building, getBuildingName, "Clasa 'Building' nu conține metoda 'getBuildingName'!");
    ASSERT_METHOD_SIGNATURE(Building, getBuildingName, qyqufkzo, "Clasa 'Building' nu conține metoda 'getBuildingName' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getCitizen) {

    ASSERT_METHOD(Building, getCitizen, "Clasa 'Building' nu conține metoda 'getCitizen'!");
    ASSERT_METHOD_SIGNATURE(Building, getCitizen, idnndqdd, "Clasa 'Building' nu conține metoda 'getCitizen' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getCitizensSize) {

    ASSERT_METHOD(Building, getCitizensSize, "Clasa 'Building' nu conține metoda 'getCitizensSize'!");
    ASSERT_METHOD_SIGNATURE(Building, getCitizensSize, zsrnaffr, "Clasa 'Building' nu conține metoda 'getCitizensSize' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getCoordinator) {

    ASSERT_METHOD(Building, getCoordinator, "Clasa 'Building' nu conține metoda 'getCoordinator'!");
    ASSERT_METHOD_SIGNATURE(Building, getCoordinator, ctjisbzd, "Clasa 'Building' nu conține metoda 'getCoordinator' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getEmployee) {

    ASSERT_METHOD(Building, getEmployee, "Clasa 'Building' nu conține metoda 'getEmployee'!");
    ASSERT_METHOD_SIGNATURE(Building, getEmployee, xdqgiomk, "Clasa 'Building' nu conține metoda 'getEmployee' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getEmployeesSize) {

    ASSERT_METHOD(Building, getEmployeesSize, "Clasa 'Building' nu conține metoda 'getEmployeesSize'!");
    ASSERT_METHOD_SIGNATURE(Building, getEmployeesSize, iskworls, "Clasa 'Building' nu conține metoda 'getEmployeesSize' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, getType) {

    ASSERT_METHOD(Building, getType, "Clasa 'Building' nu conține metoda 'getType'!");
    ASSERT_METHOD_SIGNATURE(Building, getType, xfnnjivx, "Clasa 'Building' nu conține metoda 'getType' cu tipul specificat în cerință!");

}

TEST(BuildingMethod, remove) {

    ASSERT_METHOD(Building, remove, "Clasa 'Building' nu conține metoda 'remove'!");
    ASSERT_METHOD_SIGNATURE(Building, remove, eqvwuojb, "Clasa 'Building' nu conține metoda 'remove' cu tipul specificat în cerință!");

}

TEST(CityHallMethod, add) {

    ASSERT_METHOD(CityHall, add, "Clasa 'CityHall' nu conține metoda 'add'!");
    ASSERT_METHOD_SIGNATURE(CityHall, add, npvinakx, "Clasa 'CityHall' nu conține metoda 'add' cu tipul specificat în cerință!");

}

TEST(CityHallMethod, remove) {

    ASSERT_METHOD(CityHall, remove, "Clasa 'CityHall' nu conține metoda 'remove'!");
    ASSERT_METHOD_SIGNATURE(CityHall, remove, nojuxurg, "Clasa 'CityHall' nu conține metoda 'remove' cu tipul specificat în cerință!");

}

TEST(HospitalMethod, add) {

    ASSERT_METHOD(Hospital, add, "Clasa 'Hospital' nu conține metoda 'add'!");
    ASSERT_METHOD_SIGNATURE(Hospital, add, kipduwru, "Clasa 'Hospital' nu conține metoda 'add' cu tipul specificat în cerință!");

}

TEST(HospitalMethod, remove) {

    ASSERT_METHOD(Hospital, remove, "Clasa 'Hospital' nu conține metoda 'remove'!");
    ASSERT_METHOD_SIGNATURE(Hospital, remove, beyanvbm, "Clasa 'Hospital' nu conține metoda 'remove' cu tipul specificat în cerință!");

}

TEST(HouseMethod, add) {

    ASSERT_METHOD(House, add, "Clasa 'House' nu conține metoda 'add'!");
    ASSERT_METHOD_SIGNATURE(House, add, pihdhpgp, "Clasa 'House' nu conține metoda 'add' cu tipul specificat în cerință!");

}

TEST(HouseMethod, remove) {

    ASSERT_METHOD(House, remove, "Clasa 'House' nu conține metoda 'remove'!");
    ASSERT_METHOD_SIGNATURE(House, remove, eeybzznm, "Clasa 'House' nu conține metoda 'remove' cu tipul specificat în cerință!");

}

TEST(PoliceStationMethod, add) {

    ASSERT_METHOD(PoliceStation, add, "Clasa 'PoliceStation' nu conține metoda 'add'!");
    ASSERT_METHOD_SIGNATURE(PoliceStation, add, ydejvdbh, "Clasa 'PoliceStation' nu conține metoda 'add' cu tipul specificat în cerință!");

}

TEST(PoliceStationMethod, remove) {

    ASSERT_METHOD(PoliceStation, remove, "Clasa 'PoliceStation' nu conține metoda 'remove'!");
    ASSERT_METHOD_SIGNATURE(PoliceStation, remove, pqwuccaa, "Clasa 'PoliceStation' nu conține metoda 'remove' cu tipul specificat în cerință!");

}

TEST(TestMethod, print) {

    ASSERT_METHOD(Test, print, "Clasa 'Test' nu conține metoda 'print'!");
    ASSERT_METHOD_SIGNATURE(Test, print, aqxcsumc, "Clasa 'Test' nu conține metoda 'print' cu tipul specificat în cerință!");

}

TEST(TestMethod, sum) {

    ASSERT_METHOD(Test, sum, "Clasa 'Test' nu conține metoda 'sum'!");
    ASSERT_METHOD_SIGNATURE(Test, sum, oexhjhap, "Clasa 'Test' nu conține metoda 'sum' cu tipul specificat în cerință!");

}

TEST(BlockOperator, operatorPlus) {

    ASSERT_OPERATOR(Block, operatorPlus, "Clasa 'Block' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(Block, operatorPlus, vrocytuc, "Clasa 'Block' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(BlockOperator, operatorMinus) {

    ASSERT_OPERATOR(Block, operatorMinus, "Clasa 'Block' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(Block, operatorMinus, czidquey, "Clasa 'Block' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(BuildingOperator, operatorPlus) {

    ASSERT_OPERATOR(Building, operatorPlus, "Clasa 'Building' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(Building, operatorPlus, fxblakqi, "Clasa 'Building' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(BuildingOperator, operatorMinus) {

    ASSERT_OPERATOR(Building, operatorMinus, "Clasa 'Building' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(Building, operatorMinus, ofunvjdh, "Clasa 'Building' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(BuildingOperator, operatorMinus2) {

    ASSERT_OPERATOR(Building, operatorMinus, "Clasa 'Building' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(Building, operatorMinus, cypmhicg, "Clasa 'Building' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(CityHallOperator, operatorPlus) {

    ASSERT_OPERATOR(CityHall, operatorPlus, "Clasa 'CityHall' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(CityHall, operatorPlus, fakzcxhp, "Clasa 'CityHall' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(CityHallOperator, operatorMinus) {

    ASSERT_OPERATOR(CityHall, operatorMinus, "Clasa 'CityHall' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(CityHall, operatorMinus, gbwjlndw, "Clasa 'CityHall' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(HospitalOperator, operatorPlus) {

    ASSERT_OPERATOR(Hospital, operatorPlus, "Clasa 'Hospital' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(Hospital, operatorPlus, slzytbms, "Clasa 'Hospital' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(HospitalOperator, operatorMinus) {

    ASSERT_OPERATOR(Hospital, operatorMinus, "Clasa 'Hospital' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(Hospital, operatorMinus, zgoyqlmc, "Clasa 'Hospital' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(HouseOperator, operatorPlus) {

    ASSERT_OPERATOR(House, operatorPlus, "Clasa 'House' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(House, operatorPlus, nzkfiyod, "Clasa 'House' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(HouseOperator, operatorMinus) {

    ASSERT_OPERATOR(House, operatorMinus, "Clasa 'House' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(House, operatorMinus, qihxinex, "Clasa 'House' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(PersonOperator, operatorLess) {

    ASSERT_OPERATOR(Person, operatorLess, "Clasa 'Person' nu conține operatorul '<'!");
    ASSERT_OPERATOR_SIGNATURE(Person, operatorLess, kdfngvaz, "Clasa 'Person' nu conține operatorul '<' cu tipul specificat în cerință!");

}

TEST(PersonOperator, operatorEqual) {

    ASSERT_OPERATOR(Person, operatorEqual, "Clasa 'Person' nu conține operatorul '=='!");
    ASSERT_OPERATOR_SIGNATURE(Person, operatorEqual, fqcdvuib, "Clasa 'Person' nu conține operatorul '==' cu tipul specificat în cerință!");

}

TEST(PoliceStationOperator, operatorPlus) {

    ASSERT_OPERATOR(PoliceStation, operatorPlus, "Clasa 'PoliceStation' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(PoliceStation, operatorPlus, ayijppwo, "Clasa 'PoliceStation' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(PoliceStationOperator, operatorMinus) {

    ASSERT_OPERATOR(PoliceStation, operatorMinus, "Clasa 'PoliceStation' nu conține operatorul '-'!");
    ASSERT_OPERATOR_SIGNATURE(PoliceStation, operatorMinus, glejjnse, "Clasa 'PoliceStation' nu conține operatorul '-' cu tipul specificat în cerință!");

}

TEST(TestOperator, operatorPlus) {

    ASSERT_OPERATOR(Test, operatorPlus, "Clasa 'Test' nu conține operatorul '+'!");
    ASSERT_OPERATOR_SIGNATURE(Test, operatorPlus, crdnycgl, "Clasa 'Test' nu conține operatorul '+' cu tipul specificat în cerință!");

}

TEST(Constructor, 3Param_Block) {

    ASSERT_CONSTRUCTOR("Constructorul cu 3 param. pentru clasa 'Block' nu există!", Block, std::string, uint16_t, uint16_t) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 0Param_Building) {

    ASSERT_CONSTRUCTOR("Constructorul cu 0 param. pentru clasa 'Building' nu există!", Building) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 2Param_Building) {

    ASSERT_CONSTRUCTOR("Constructorul cu 2 param. pentru clasa 'Building' nu există!", Building, std::string, std::string) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 1Param_CityHall) {

    ASSERT_CONSTRUCTOR("Constructorul cu 1 param. pentru clasa 'CityHall' nu există!", CityHall, std::string) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 1Param_Hospital) {

    ASSERT_CONSTRUCTOR("Constructorul cu 1 param. pentru clasa 'Hospital' nu există!", Hospital, std::string) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 2Param_House) {

    ASSERT_CONSTRUCTOR("Constructorul cu 2 param. pentru clasa 'House' nu există!", House, std::string, uint16_t) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 0Param_Person) {

    ASSERT_CONSTRUCTOR("Constructorul cu 0 param. pentru clasa 'Person' nu există!", Person) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 3Param_Person) {

    ASSERT_CONSTRUCTOR("Constructorul cu 3 param. pentru clasa 'Person' nu există!", Person, std::string, uint8_t, double) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 1Param_PoliceStation) {

    ASSERT_CONSTRUCTOR("Constructorul cu 1 param. pentru clasa 'PoliceStation' nu există!", PoliceStation, std::string) BEGIN {
        /* insert code */
    } END

}

TEST(Constructor, 0Param_Test) {

    ASSERT_CONSTRUCTOR("Constructorul cu 0 param. pentru clasa 'Test' nu există!", Test) BEGIN {
        Test test_default;
		auto tmp = TestWrapper::tmp(&test_default);
		auto ftmp = TestWrapper::ftmp(&test_default);
		auto sum = TestWrapper::sum(&test_default, 2, 3);
		ASSERT_EQUAL(tmp, 10, "Constructorul cu 0 param. pentru clasa 'Test' nu initializeaza corect atributul 'tmp'!");
		ASSERT_EQUAL(ftmp, 12.45f, "Constructorul cu 0 param. pentru clasa 'Test' nu initializeaza corect atributul 'ftmp'!");
		ASSERT_EQUAL(sum, 15, "Metoda 'sum' din clasa 'Test' nu întoarce rezultatul corect!");
    } END

}

TEST(Constructor, 2Param_Test) {

    ASSERT_CONSTRUCTOR("Constructorul cu 2 param. pentru clasa 'Test' nu există!", Test, int, float) BEGIN {
        Test test_int_float(1, 1.5f);
		auto tmp = TestWrapper::tmp(&test_int_float);
		auto ftmp = TestWrapper::ftmp(&test_int_float);
		auto sum = TestWrapper::sum(&test_int_float, 2, 3);
		ASSERT_EQUAL(tmp, 1, "Constructorul cu 2 param. pentru clasa 'Test' nu initializeaza corect atributul 'tmp'!");
		ASSERT_EQUAL(ftmp, 1.5f, "Constructorul cu 2 param. pentru clasa 'Test' nu initializeaza corect atributul 'ftmp'!!");
		ASSERT_EQUAL(sum, 6, "Metoda 'sum' din clasa 'Test' nu întoarce rezultatul corect!");
    } END

}

