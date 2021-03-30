/**
  * Unit Test Framework for compile time checking in VPL
  * @Copyright (C) 2020 Andrei-Edward Popa
  * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
  */

REFLECT_CLASS(Block) {
    REFLECT_ATTRIBUTE(mMaxNumberPerFloor, uint16_t);
    REFLECT_METHOD(add, void, const Person &, /* Method Qualifiers */ );
    REFLECT_METHOD(remove, void, const Person &, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Building*, const Person &, /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus, Building*,  /* Operator Qualifiers */ );
};

REFLECT_CLASS(Building) {
    REFLECT_ATTRIBUTE(mBuildingName, std::string);
    REFLECT_ATTRIBUTE(mCitizens, std::vector<Person>);
    REFLECT_ATTRIBUTE(mCoordinator, Person);
    REFLECT_ATTRIBUTE(mEmployees, std::vector<Person>);
    REFLECT_ATTRIBUTE(mType, std::string);
    REFLECT_METHOD(add, void, const Person &, /* Method Qualifiers */ );
    REFLECT_METHOD(getBuildingName, std::string,  /* Method Qualifiers */ const);
    REFLECT_METHOD(getCitizen, Person, uint16_t, /* Method Qualifiers */  const);
    REFLECT_METHOD(getCitizensSize, uint16_t,  /* Method Qualifiers */ const);
    REFLECT_METHOD(getCoordinator, Person,  /* Method Qualifiers */ const);
    REFLECT_METHOD(getEmployee, Person, uint16_t, /* Method Qualifiers */  const);
    REFLECT_METHOD(getEmployeesSize, uint16_t,  /* Method Qualifiers */ const);
    REFLECT_METHOD(getType, std::string,  /* Method Qualifiers */ const);
    REFLECT_METHOD(remove, void, const Person &, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Building*, const Person &, /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus, Building*,  /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus2, Building*, const Person &, /* Operator Qualifiers */ );
};

REFLECT_CLASS(CityHall) {
    REFLECT_METHOD(add, void, const Person &, /* Method Qualifiers */ );
    REFLECT_METHOD(remove, void, const Person &, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Building*, const Person &, /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus, Building*,  /* Operator Qualifiers */ );
};

REFLECT_CLASS(Hospital) {
    REFLECT_METHOD(add, void, const Person &, /* Method Qualifiers */ );
    REFLECT_METHOD(remove, void, const Person &, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Building*, const Person &, /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus, Building*, const Person &, /* Operator Qualifiers */ );
};

REFLECT_CLASS(House) {
    REFLECT_ATTRIBUTE(mMaxNumber, uint16_t);
    REFLECT_METHOD(add, void, const Person &, /* Method Qualifiers */ );
    REFLECT_METHOD(remove, void, const Person &, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Building*, const Person &, /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus, Building*,  /* Operator Qualifiers */ );
};

REFLECT_CLASS(Person) {
    REFLECT_ATTRIBUTE(mAge, uint8_t);
    REFLECT_ATTRIBUTE(mName, std::string);
    REFLECT_ATTRIBUTE(mSalary, double);
    REFLECT_OPERATOR(operator<, operatorLess, bool, const Person & /* Operator Qualifiers */ , const);
    REFLECT_OPERATOR(operator==, operatorEqual, bool, const Person & /* Operator Qualifiers */ , const);
};

REFLECT_CLASS(PoliceStation) {
    REFLECT_METHOD(add, void, const Person &, /* Method Qualifiers */ );
    REFLECT_METHOD(remove, void, const Person &, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Building*, const Person &, /* Operator Qualifiers */ );
    REFLECT_OPERATOR(operator-, operatorMinus, Building*, const Person &, /* Operator Qualifiers */ );
};

REFLECT_CLASS(Test) {
    REFLECT_ATTRIBUTE(ftmp, float);
    REFLECT_ATTRIBUTE(tmp, int);
    REFLECT_METHOD(print, void,  /* Method Qualifiers */ );
    REFLECT_METHOD(sum, int, int , int, /* Method Qualifiers */ );
    REFLECT_OPERATOR(operator+, operatorPlus, Test, const Test &, /* Operator Qualifiers */ );
};

REFLECT_FUNCTION(newPerson, GlobalFunctionsWrapper::Person, const int , int , GlobalFunctionsWrapper::Block &, const double *);
REFLECT_FUNCTION(multiply, int, int , int);

TEST(Segmentation, Fault) {
    /* insert code */
}

TEST(Function, multiply) {

    ASSERT_FUNCTION(multiply, "Funcția 'multiply' nu există sau nu conține tipul specificat în cerință!") BEGIN {
        /* insert code */
    } END

}

TEST(Function, newPerson) {

    ASSERT_FUNCTION(newPerson, "Funcția 'newPerson' nu există sau nu conține tipul specificat în cerință!") BEGIN {
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

    ASSERT_ATTRIBUTE(Block, mMaxNumberPerFloor, "Clasa 'Block' nu conține atributul 'mMaxNumberPerFloor' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingAttribute, mBuildingName) {

    ASSERT_ATTRIBUTE(Building, mBuildingName, "Clasa 'Building' nu conține atributul 'mBuildingName' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingAttribute, mCitizens) {

    ASSERT_ATTRIBUTE(Building, mCitizens, "Clasa 'Building' nu conține atributul 'mCitizens' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingAttribute, mCoordinator) {

    ASSERT_ATTRIBUTE(Building, mCoordinator, "Clasa 'Building' nu conține atributul 'mCoordinator' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingAttribute, mEmployees) {

    ASSERT_ATTRIBUTE(Building, mEmployees, "Clasa 'Building' nu conține atributul 'mEmployees' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingAttribute, mType) {

    ASSERT_ATTRIBUTE(Building, mType, "Clasa 'Building' nu conține atributul 'mType' sau tipul specificat în cerință este incorect!");
}

TEST(HouseAttribute, mMaxNumber) {

    ASSERT_ATTRIBUTE(House, mMaxNumber, "Clasa 'House' nu conține atributul 'mMaxNumber' sau tipul specificat în cerință este incorect!");
}

TEST(PersonAttribute, mAge) {

    ASSERT_ATTRIBUTE(Person, mAge, "Clasa 'Person' nu conține atributul 'mAge' sau tipul specificat în cerință este incorect!");
}

TEST(PersonAttribute, mName) {

    ASSERT_ATTRIBUTE(Person, mName, "Clasa 'Person' nu conține atributul 'mName' sau tipul specificat în cerință este incorect!");
}

TEST(PersonAttribute, mSalary) {

    ASSERT_ATTRIBUTE(Person, mSalary, "Clasa 'Person' nu conține atributul 'mSalary' sau tipul specificat în cerință este incorect!");
}

TEST(TestAttribute, ftmp) {

    ASSERT_ATTRIBUTE(Test, ftmp, "Clasa 'Test' nu conține atributul 'ftmp' sau tipul specificat în cerință este incorect!");
}

TEST(TestAttribute, tmp) {

    ASSERT_ATTRIBUTE(Test, tmp, "Clasa 'Test' nu conține atributul 'tmp' sau tipul specificat în cerință este incorect!");
}

TEST(BlockMethod, add) {

    ASSERT_METHOD(Block, add, "Clasa 'Block' nu conține metoda 'add' sau tipul specificat în cerință este incorect!");
}

TEST(BlockMethod, remove) {

    ASSERT_METHOD(Block, remove, "Clasa 'Block' nu conține metoda 'remove' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, add) {

    ASSERT_METHOD(Building, add, "Clasa 'Building' nu conține metoda 'add' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getBuildingName) {

    ASSERT_METHOD(Building, getBuildingName, "Clasa 'Building' nu conține metoda 'getBuildingName' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getCitizen) {

    ASSERT_METHOD(Building, getCitizen, "Clasa 'Building' nu conține metoda 'getCitizen' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getCitizensSize) {

    ASSERT_METHOD(Building, getCitizensSize, "Clasa 'Building' nu conține metoda 'getCitizensSize' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getCoordinator) {

    ASSERT_METHOD(Building, getCoordinator, "Clasa 'Building' nu conține metoda 'getCoordinator' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getEmployee) {

    ASSERT_METHOD(Building, getEmployee, "Clasa 'Building' nu conține metoda 'getEmployee' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getEmployeesSize) {

    ASSERT_METHOD(Building, getEmployeesSize, "Clasa 'Building' nu conține metoda 'getEmployeesSize' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, getType) {

    ASSERT_METHOD(Building, getType, "Clasa 'Building' nu conține metoda 'getType' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingMethod, remove) {

    ASSERT_METHOD(Building, remove, "Clasa 'Building' nu conține metoda 'remove' sau tipul specificat în cerință este incorect!");
}

TEST(CityHallMethod, add) {

    ASSERT_METHOD(CityHall, add, "Clasa 'CityHall' nu conține metoda 'add' sau tipul specificat în cerință este incorect!");
}

TEST(CityHallMethod, remove) {

    ASSERT_METHOD(CityHall, remove, "Clasa 'CityHall' nu conține metoda 'remove' sau tipul specificat în cerință este incorect!");
}

TEST(HospitalMethod, add) {

    ASSERT_METHOD(Hospital, add, "Clasa 'Hospital' nu conține metoda 'add' sau tipul specificat în cerință este incorect!");
}

TEST(HospitalMethod, remove) {

    ASSERT_METHOD(Hospital, remove, "Clasa 'Hospital' nu conține metoda 'remove' sau tipul specificat în cerință este incorect!");
}

TEST(HouseMethod, add) {

    ASSERT_METHOD(House, add, "Clasa 'House' nu conține metoda 'add' sau tipul specificat în cerință este incorect!");
}

TEST(HouseMethod, remove) {

    ASSERT_METHOD(House, remove, "Clasa 'House' nu conține metoda 'remove' sau tipul specificat în cerință este incorect!");
}

TEST(PoliceStationMethod, add) {

    ASSERT_METHOD(PoliceStation, add, "Clasa 'PoliceStation' nu conține metoda 'add' sau tipul specificat în cerință este incorect!");
}

TEST(PoliceStationMethod, remove) {

    ASSERT_METHOD(PoliceStation, remove, "Clasa 'PoliceStation' nu conține metoda 'remove' sau tipul specificat în cerință este incorect!");
}

TEST(TestMethod, print) {

    ASSERT_METHOD(Test, print, "Clasa 'Test' nu conține metoda 'print' sau tipul specificat în cerință este incorect!");
}

TEST(TestMethod, sum) {

    ASSERT_METHOD(Test, sum, "Clasa 'Test' nu conține metoda 'sum' sau tipul specificat în cerință este incorect!");
}

TEST(BlockOperator, operatorPlus) {

    ASSERT_OPERATOR(Block, operatorPlus, "Clasa 'Block' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(BlockOperator, operatorMinus) {

    ASSERT_OPERATOR(Block, operatorMinus, "Clasa 'Block' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingOperator, operatorPlus) {

    ASSERT_OPERATOR(Building, operatorPlus, "Clasa 'Building' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingOperator, operatorMinus) {

    ASSERT_OPERATOR(Building, operatorMinus, "Clasa 'Building' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(BuildingOperator, operatorMinus2) {

    ASSERT_OPERATOR(Building, operatorMinus2, "Clasa 'Building' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(CityHallOperator, operatorPlus) {

    ASSERT_OPERATOR(CityHall, operatorPlus, "Clasa 'CityHall' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(CityHallOperator, operatorMinus) {

    ASSERT_OPERATOR(CityHall, operatorMinus, "Clasa 'CityHall' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(HospitalOperator, operatorPlus) {

    ASSERT_OPERATOR(Hospital, operatorPlus, "Clasa 'Hospital' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(HospitalOperator, operatorMinus) {

    ASSERT_OPERATOR(Hospital, operatorMinus, "Clasa 'Hospital' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(HouseOperator, operatorPlus) {

    ASSERT_OPERATOR(House, operatorPlus, "Clasa 'House' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(HouseOperator, operatorMinus) {

    ASSERT_OPERATOR(House, operatorMinus, "Clasa 'House' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(PersonOperator, operatorLess) {

    ASSERT_OPERATOR(Person, operatorLess, "Clasa 'Person' nu conține operatorul 'operatorLess' sau tipul specificat în cerință este incorect!");
}

TEST(PersonOperator, operatorEqual) {

    ASSERT_OPERATOR(Person, operatorEqual, "Clasa 'Person' nu conține operatorul 'operatorEqual' sau tipul specificat în cerință este incorect!");
}

TEST(PoliceStationOperator, operatorPlus) {

    ASSERT_OPERATOR(PoliceStation, operatorPlus, "Clasa 'PoliceStation' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(PoliceStationOperator, operatorMinus) {

    ASSERT_OPERATOR(PoliceStation, operatorMinus, "Clasa 'PoliceStation' nu conține operatorul 'operatorMinus' sau tipul specificat în cerință este incorect!");
}

TEST(TestOperator, operatorPlus) {

    ASSERT_OPERATOR(Test, operatorPlus, "Clasa 'Test' nu conține operatorul 'operatorPlus' sau tipul specificat în cerință este incorect!");
}

TEST(Constructor, 3Param_Block) {

    ASSERT_CONSTRUCTOR("Constructorul cu 3 param. pentru clasa 'Block' nu există!", Block, std::string, uint16_t, uint16_t) BEGIN {
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

