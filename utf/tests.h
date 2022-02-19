/**
  * Unit Test Framework for compile time checking in VPL
  * @Copyright (C) 2020 Andrei-Edward Popa
  * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
  */

REGISTER_CLASS(Person) {
    REGISTER_ATTRIBUTE(mAge, uint8_t, cmykqwwu);
    REGISTER_ATTRIBUTE(mName, std::string, kvbjjqyl);
    REGISTER_ATTRIBUTE(mSalary, double, ghknfbdg);
    REGISTER_OPERATOR(operator<, operatorLess, bool(Class::*)(const Person &) const, qqcyqlzc);
    REGISTER_OPERATOR(operator==, operatorEqual, bool(Class::*)(const Person &) const, zledaezq);
};

REGISTER_CLASS(Building) {
    REGISTER_ATTRIBUTE(mBuildingName, std::string, ukiqaplf);
    REGISTER_ATTRIBUTE(mCitizens, std::vector<Person>, swznhdhy);
    REGISTER_ATTRIBUTE(mCoordinator, Person, wwyywumt);
    REGISTER_ATTRIBUTE(mEmployees, std::vector<Person>, tpefyqll);
    REGISTER_ATTRIBUTE(mType, std::string, thtlyxhy);
    REGISTER_METHOD(add, void(Class::*)(const Person &), sydajyxb);
    REGISTER_METHOD(getBuildingName, std::string(Class::*)() const, ksftdjdo);
    REGISTER_METHOD(getCitizen, Person(Class::*)(uint16_t) const, lqbjenut);
    REGISTER_METHOD(getCitizensSize, uint16_t(Class::*)() const, pidhuben);
    REGISTER_METHOD(getCoordinator, Person(Class::*)() const, pncvgbwj);
    REGISTER_METHOD(getEmployee, Person(Class::*)(uint16_t) const, txvwkkgu);
    REGISTER_METHOD(getEmployeesSize, uint16_t(Class::*)() const, lrmsclnj);
    REGISTER_METHOD(getType, std::string(Class::*)() const, qmohsdmp);
    REGISTER_METHOD(remove, void(Class::*)(const Person &), fewiezlq);
    REGISTER_OPERATOR(operator+, operatorPlus, Building*(Class::*)(const Person &), bpjctkyy);
    REGISTER_OPERATOR(operator-, operatorMinus, Building*(Class::*)(), xyecuekp);
    REGISTER_SAME_OPERATOR(operator-, operatorMinus, Building*(Class::*)(const Person &), nnzipqyq);
};

REGISTER_CLASS(Block) {
    REGISTER_ATTRIBUTE(mMaxNumberPerFloor, uint16_t, yaxtfwwj);
    REGISTER_METHOD(add, void(Class::*)(const Person &), zklylbsq);
    REGISTER_METHOD(remove, void(Class::*)(const Person &), okllazis);
    REGISTER_OPERATOR(operator+, operatorPlus, Building*(Class::*)(const Person &), gzkbhqlr);
    REGISTER_OPERATOR(operator-, operatorMinus, Building*(Class::*)(), mhsdefeo);
};

REGISTER_CLASS(CityHall) {
    REGISTER_METHOD(add, void(Class::*)(const Person &), jqffcfiu);
    REGISTER_METHOD(remove, void(Class::*)(const Person &), zdtbwcxw);
    REGISTER_OPERATOR(operator+, operatorPlus, Building*(Class::*)(const Person &), xkihroxa);
    REGISTER_OPERATOR(operator-, operatorMinus, Building*(Class::*)(), hvikhqax);
};

REGISTER_CLASS(Hospital) {
    REGISTER_METHOD(add, void(Class::*)(const Person &), eqlebpzz);
    REGISTER_METHOD(remove, void(Class::*)(const Person &), xqyscaem);
    REGISTER_OPERATOR(operator+, operatorPlus, Building*(Class::*)(const Person &), lgodagry);
    REGISTER_OPERATOR(operator-, operatorMinus, Building*(Class::*)(const Person &), lkbkautl);
};

REGISTER_CLASS(House) {
    REGISTER_ATTRIBUTE(mMaxNumber, uint16_t, dtbgbkdg);
    REGISTER_METHOD(add, void(Class::*)(const Person &), bqqylznb);
    REGISTER_METHOD(remove, void(Class::*)(const Person &), ntxvhnol);
    REGISTER_OPERATOR(operator+, operatorPlus, Building*(Class::*)(const Person &), ujvptyie);
    REGISTER_OPERATOR(operator-, operatorMinus, Building*(Class::*)(), rcvecrcx);
};

REGISTER_CLASS(PoliceStation) {
    REGISTER_METHOD(add, void(Class::*)(const Person &), pitpnnga);
    REGISTER_METHOD(remove, void(Class::*)(const Person &), exsrhnow);
    REGISTER_OPERATOR(operator+, operatorPlus, Building*(Class::*)(const Person &), vgdumgme);
    REGISTER_OPERATOR(operator-, operatorMinus, Building*(Class::*)(const Person &), vexoyegv);
};

REGISTER_CLASS(Test) {
    REGISTER_ATTRIBUTE(ftmp, float, wkgdsmdo);
    REGISTER_ATTRIBUTE(tmp, int, uagysbtm);
    REGISTER_METHOD(print, void(Class::*)(), iflgiaua);
    REGISTER_METHOD(sum, int(Class::*)(int, int), zykvvltz);
    REGISTER_OPERATOR(operator+, operatorPlus, Test(Class::*)(const Test &), ojltzwpt);
};

REGISTER_FUNCTION(Person, newPerson, const int, int, Block &, const double *, rtgrgyjp);
REGISTER_FUNCTION(int, multiply, int, int, fwvrzxop);

TEST(private, SegmentationFault) {
	REGISTER_SEGMENTATION_FAULT("Test #8", "Message for test 8");
}

TEST(Function, multiply) {

    ASSERT_FUNCTION(multiply, fwvrzxop, "Funcția 'multiply' nu există sau nu s-a respectat signatura specificată în cerință!") {
        /* insert code */
    }

}

TEST(Function, newPerson) {

    ASSERT_FUNCTION(newPerson, rtgrgyjp, "Funcția 'newPerson' nu există sau nu s-a respectat signatura specificată în cerință!") {
        /* insert code */
    }

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

    ASSERT_DESTRUCTOR(Block, "Clasa 'Block' nu conține un destructor!");

}

TEST(Destructor, Building) {

    ASSERT_DESTRUCTOR(Building, "Clasa 'Building' nu conține un destructor!");

}

TEST(Destructor, CityHall) {

    ASSERT_DESTRUCTOR(CityHall, "Clasa 'CityHall' nu conține un destructor!");

}

TEST(Destructor, Hospital) {

    ASSERT_DESTRUCTOR(Hospital, "Clasa 'Hospital' nu conține un destructor!");

}

TEST(Destructor, House) {

    ASSERT_DESTRUCTOR(House, "Clasa 'House' nu conține un destructor!");

}

TEST(Destructor, Person) {

    ASSERT_DESTRUCTOR(Person, "Clasa 'Person' nu conține un destructor!");

}

TEST(Destructor, PoliceStation) {

    ASSERT_DESTRUCTOR(PoliceStation, "Clasa 'PoliceStation' nu conține un destructor!");

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

    ASSERT_ATTRIBUTE(Block, mMaxNumberPerFloor, yaxtfwwj, "Clasa 'Block' nu conține atributul 'mMaxNumberPerFloor' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(BuildingAttribute, mBuildingName) {

    ASSERT_ATTRIBUTE(Building, mBuildingName, ukiqaplf, "Clasa 'Building' nu conține atributul 'mBuildingName' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(BuildingAttribute, mCitizens) {

    ASSERT_ATTRIBUTE(Building, mCitizens, swznhdhy, "Clasa 'Building' nu conține atributul 'mCitizens' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(BuildingAttribute, mCoordinator) {

    ASSERT_ATTRIBUTE(Building, mCoordinator, wwyywumt, "Clasa 'Building' nu conține atributul 'mCoordinator' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(BuildingAttribute, mEmployees) {

    ASSERT_ATTRIBUTE(Building, mEmployees, tpefyqll, "Clasa 'Building' nu conține atributul 'mEmployees' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(BuildingAttribute, mType) {

    ASSERT_ATTRIBUTE(Building, mType, thtlyxhy, "Clasa 'Building' nu conține atributul 'mType' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(HouseAttribute, mMaxNumber) {

    ASSERT_ATTRIBUTE(House, mMaxNumber, dtbgbkdg, "Clasa 'House' nu conține atributul 'mMaxNumber' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(PersonAttribute, mAge) {

    ASSERT_ATTRIBUTE(Person, mAge, cmykqwwu, "Clasa 'Person' nu conține atributul 'mAge' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(PersonAttribute, mName) {

    ASSERT_ATTRIBUTE(Person, mName, kvbjjqyl, "Clasa 'Person' nu conține atributul 'mName' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(PersonAttribute, mSalary) {

    ASSERT_ATTRIBUTE(Person, mSalary, ghknfbdg, "Clasa 'Person' nu conține atributul 'mSalary' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(TestAttribute, ftmp) {

    ASSERT_ATTRIBUTE(Test, ftmp, wkgdsmdo, "Clasa 'Test' nu conține atributul 'ftmp' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(TestAttribute, tmp) {

    ASSERT_ATTRIBUTE(Test, tmp, uagysbtm, "Clasa 'Test' nu conține atributul 'tmp' sau nu s-a respectat tipul specificat în cerință!");

}

TEST(BlockMethod, add) {

    ASSERT_METHOD(Block, add, zklylbsq, "Clasa 'Block' nu conține metoda 'add' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BlockMethod, remove) {

    ASSERT_METHOD(Block, remove, okllazis, "Clasa 'Block' nu conține metoda 'remove' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, add) {

    ASSERT_METHOD(Building, add, sydajyxb, "Clasa 'Building' nu conține metoda 'add' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getBuildingName) {

    ASSERT_METHOD(Building, getBuildingName, ksftdjdo, "Clasa 'Building' nu conține metoda 'getBuildingName' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getCitizen) {

    ASSERT_METHOD(Building, getCitizen, lqbjenut, "Clasa 'Building' nu conține metoda 'getCitizen' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getCitizensSize) {

    ASSERT_METHOD(Building, getCitizensSize, pidhuben, "Clasa 'Building' nu conține metoda 'getCitizensSize' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getCoordinator) {

    ASSERT_METHOD(Building, getCoordinator, pncvgbwj, "Clasa 'Building' nu conține metoda 'getCoordinator' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getEmployee) {

    ASSERT_METHOD(Building, getEmployee, txvwkkgu, "Clasa 'Building' nu conține metoda 'getEmployee' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getEmployeesSize) {

    ASSERT_METHOD(Building, getEmployeesSize, lrmsclnj, "Clasa 'Building' nu conține metoda 'getEmployeesSize' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, getType) {

    ASSERT_METHOD(Building, getType, qmohsdmp, "Clasa 'Building' nu conține metoda 'getType' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingMethod, remove) {

    ASSERT_METHOD(Building, remove, fewiezlq, "Clasa 'Building' nu conține metoda 'remove' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(CityHallMethod, add) {

    ASSERT_METHOD(CityHall, add, jqffcfiu, "Clasa 'CityHall' nu conține metoda 'add' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(CityHallMethod, remove) {

    ASSERT_METHOD(CityHall, remove, zdtbwcxw, "Clasa 'CityHall' nu conține metoda 'remove' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HospitalMethod, add) {

    ASSERT_METHOD(Hospital, add, eqlebpzz, "Clasa 'Hospital' nu conține metoda 'add' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HospitalMethod, remove) {

    ASSERT_METHOD(Hospital, remove, xqyscaem, "Clasa 'Hospital' nu conține metoda 'remove' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HouseMethod, add) {

    ASSERT_METHOD(House, add, bqqylznb, "Clasa 'House' nu conține metoda 'add' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HouseMethod, remove) {

    ASSERT_METHOD(House, remove, ntxvhnol, "Clasa 'House' nu conține metoda 'remove' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(PoliceStationMethod, add) {

    ASSERT_METHOD(PoliceStation, add, pitpnnga, "Clasa 'PoliceStation' nu conține metoda 'add' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(PoliceStationMethod, remove) {

    ASSERT_METHOD(PoliceStation, remove, exsrhnow, "Clasa 'PoliceStation' nu conține metoda 'remove' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(TestMethod, print) {

    ASSERT_METHOD(Test, print, iflgiaua, "Clasa 'Test' nu conține metoda 'print' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(TestMethod, sum) {

    ASSERT_METHOD(Test, sum, zykvvltz, "Clasa 'Test' nu conține metoda 'sum' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BlockOperator, operatorPlus) {

    ASSERT_OPERATOR(Block, operatorPlus, gzkbhqlr, "Clasa 'Block' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BlockOperator, operatorMinus) {

    ASSERT_OPERATOR(Block, operatorMinus, mhsdefeo, "Clasa 'Block' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingOperator, operatorPlus) {

    ASSERT_OPERATOR(Building, operatorPlus, bpjctkyy, "Clasa 'Building' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingOperator, operatorMinus) {

    ASSERT_OPERATOR(Building, operatorMinus, xyecuekp, "Clasa 'Building' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(BuildingOperator, operatorMinus2) {

    ASSERT_OPERATOR(Building, operatorMinus, nnzipqyq, "Clasa 'Building' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(CityHallOperator, operatorPlus) {

    ASSERT_OPERATOR(CityHall, operatorPlus, xkihroxa, "Clasa 'CityHall' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(CityHallOperator, operatorMinus) {

    ASSERT_OPERATOR(CityHall, operatorMinus, hvikhqax, "Clasa 'CityHall' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HospitalOperator, operatorPlus) {

    ASSERT_OPERATOR(Hospital, operatorPlus, lgodagry, "Clasa 'Hospital' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HospitalOperator, operatorMinus) {

    ASSERT_OPERATOR(Hospital, operatorMinus, lkbkautl, "Clasa 'Hospital' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HouseOperator, operatorPlus) {

    ASSERT_OPERATOR(House, operatorPlus, ujvptyie, "Clasa 'House' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(HouseOperator, operatorMinus) {

    ASSERT_OPERATOR(House, operatorMinus, rcvecrcx, "Clasa 'House' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(PersonOperator, operatorLess) {

    ASSERT_OPERATOR(Person, operatorLess, qqcyqlzc, "Clasa 'Person' nu conține operatorul '<' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(PersonOperator, operatorEqual) {

    ASSERT_OPERATOR(Person, operatorEqual, zledaezq, "Clasa 'Person' nu conține operatorul '==' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(PoliceStationOperator, operatorPlus) {

    ASSERT_OPERATOR(PoliceStation, operatorPlus, vgdumgme, "Clasa 'PoliceStation' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(PoliceStationOperator, operatorMinus) {

    ASSERT_OPERATOR(PoliceStation, operatorMinus, vexoyegv, "Clasa 'PoliceStation' nu conține operatorul '-' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(TestOperator, operatorPlus) {

    ASSERT_OPERATOR(Test, operatorPlus, ojltzwpt, "Clasa 'Test' nu conține operatorul '+' sau nu s-a respectat signatura specificată în cerință!");

}

TEST(Constructor, 3Param_Block) {

    ASSERT_CONSTRUCTOR(Block, std::string, uint16_t, uint16_t, "Constructorul cu 3 param. pentru clasa 'Block' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 0Param_Building) {

    ASSERT_CONSTRUCTOR(Building, "Constructorul cu 0 param. pentru clasa 'Building' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 2Param_Building) {

    ASSERT_CONSTRUCTOR(Building, std::string, std::string, "Constructorul cu 2 param. pentru clasa 'Building' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 1Param_CityHall) {

    ASSERT_CONSTRUCTOR(CityHall, std::string, "Constructorul cu 1 param. pentru clasa 'CityHall' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 1Param_Hospital) {

    ASSERT_CONSTRUCTOR(Hospital, std::string, "Constructorul cu 1 param. pentru clasa 'Hospital' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 2Param_House) {

    ASSERT_CONSTRUCTOR(House, std::string, uint16_t, "Constructorul cu 2 param. pentru clasa 'House' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 0Param_Person) {

    ASSERT_CONSTRUCTOR(Person, "Constructorul cu 0 param. pentru clasa 'Person' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 3Param_Person) {

    ASSERT_CONSTRUCTOR(Person, std::string, uint8_t, double, "Constructorul cu 3 param. pentru clasa 'Person' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 1Param_PoliceStation) {

    ASSERT_CONSTRUCTOR(PoliceStation, std::string, "Constructorul cu 1 param. pentru clasa 'PoliceStation' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
        /* insert code */
    }

}

TEST(Constructor, 0Param_Test) {

    ASSERT_CONSTRUCTOR(Test, "Constructorul cu 0 param. pentru clasa 'Test' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
		Test test_default;
		auto tmp = TestWrapper::tmp(&test_default);
		auto ftmp = TestWrapper::ftmp(&test_default);
		auto sum = TestWrapper::sum(&test_default, 2, 3);
		ASSERT_EQUAL(tmp, 10, "Constructorul cu 0 param. pentru clasa 'Test' nu initializeaza corect atributul 'tmp'!");
		ASSERT_EQUAL(ftmp, 12.45f, "Constructorul cu 0 param. pentru clasa 'Test' nu initializeaza corect atributul 'ftmp'!");
		ASSERT_EQUAL(sum, 15, "Metoda 'sum' din clasa 'Test' nu întoarce rezultatul corect!");
    }

}

TEST(Constructor, 2Param_Test) {

    ASSERT_CONSTRUCTOR(Test, int, float, "Constructorul cu 2 param. pentru clasa 'Test' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!") {
		Test test_int_float(1, 1.5f);
		auto tmp = TestWrapper::tmp(&test_int_float);
		auto ftmp = TestWrapper::ftmp(&test_int_float);
		auto sum = TestWrapper::sum(&test_int_float, 2, 3);
		ASSERT_EQUAL(tmp, 1, "Constructorul cu 2 param. pentru clasa 'Test' nu initializeaza corect atributul 'tmp'!");
		ASSERT_EQUAL(ftmp, 1.5f, "Constructorul cu 2 param. pentru clasa 'Test' nu initializeaza corect atributul 'ftmp'!!");
		ASSERT_EQUAL(sum, 6, "Metoda 'sum' din clasa 'Test' nu întoarce rezultatul corect!");
    }

}

