/**
 * VPL builtin program for submissions evaluation
 * @Copyright (C) 2012 Juan Carlos Rodríguez-del-Pino
 * @License http://www.gnu.org/copyleft/gpl.html GNU GPL v3 or later
 * @Author Juan Carlos Rodríguez-del-Pino <jcrodriguez@dis.ulpgc.es>
 */
#include "functions.h"
#include "tests.h"
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>
#include <unistd.h>
#include <pty.h>
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <execinfo.h>
#include <regex.h>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

const int MAXCOMMENTS = 20;
const int MAXCOMMENTSLENGTH = 100*1024;
const int MAXCOMMENTSTITLELENGTH = 1024;
const int MAXOUTPUT = 256* 1024 ;//256Kb
const char* TESTCASEFILE = "evaluate.cases";

/**
 * Class Tools Declaration
 */
class Tools {
public:
	static bool existFile(string name);
	static string readFile(string name);
	static vector<string> splitLines(const string &data);
	static int nextLine(const string &data);
	static string caseFormat(string text);
	static string toLower(const string &text);
	static string normalizeTag(const string &text);
	static string trimRight(const string &text);
	static string trim(const string &text);
	static void fdblock(int fd, bool set);
	static bool convert2(const string& str, double &data);
	static bool convert2(const string& str, long int &data);
};



/**
 * Class Stop Declaration
 */
class Stop{
	static volatile bool TERMRequested;
public:
	static void setTERMRequested();
	static bool isTERMRequested();
};



/**
 * Class Timer Declaration
 */
class Timer{
	static time_t startTime;
public:
	static void start();
	static int elapsedTime();
};



/**
 * Class I18n Declaration
 */
class I18n{ // No creo que merezca la pena reducir esta
public:
	void init();
	const char *get_string(const char *s);
};



/**
 * Interface OutputChecker
 */
class OutputChecker{
protected:
	string text;

public:
	OutputChecker(const string &t):text(t){}
	virtual ~OutputChecker(){};
	virtual string type(){return "";}
	virtual string outputExpected(){return text;}
	virtual string studentOutputExpected(){return text;}
	virtual bool match(const string&)=0;
	virtual OutputChecker* clone()=0;
};



/**
 * Class NumbersOutput Declaration
 */
class NumbersOutput:public OutputChecker{
	struct Number{
		bool isInteger;
		long int integer;
		double cientific;

		bool set(const string& str);
		bool operator==(const Number &o)const;
		bool operator!=(const Number &o)const;
	};

	vector<Number> numbers;
	bool startWithAsterisk;
	string cleanText;

	static bool isNum(char c);
	static bool isNumStart(char c);
	bool calcStartWithAsterisk();

public:
	NumbersOutput(const string &text);//:OutputChecker(text);
	string studentOutputExpected();
	bool operator==(const NumbersOutput& o)const;
	bool match(const string& output);
	OutputChecker* clone();
	static bool typeMatch(const string& text);
	string type();
};



/**
 * Class TextOutput Declaration
 */
class TextOutput:public OutputChecker{
	vector<string> tokens;
	bool isAlpha(char c);

public:
	TextOutput(const string &text);//:OutputChecker(text);
	bool operator==(const TextOutput& o);
	bool match(const string& output);
	OutputChecker* clone();
	static bool typeMatch(const string& text);
	string type();
};



/**
 * Class ExactTextOutput Declaration
 */
class ExactTextOutput:public OutputChecker{
	string cleanText;
	bool startWithAsterix;
	bool isAlpha(char c);

public:
	ExactTextOutput(const string &text);//:OutputChecker(text);
	string studentOutputExpected();
	bool operator==(const ExactTextOutput& o);
	bool match(const string& output);
	OutputChecker* clone();
	static bool typeMatch(const string& text);
	string type();
};



/**
 * Class RegularExpressionOutput Declaration
 * Regular Expressions implemented by:
 * Daniel José Ojeda Loisel
 * Juan David Vega Rodríguez
 * Miguel Ángel Viera González
 */
class RegularExpressionOutput:public OutputChecker {
	string errorCase;
	string cleanText;
	regex_t expression;
	bool flagI;
	bool flagM;
	int reti;

public:
	RegularExpressionOutput (const string &text, const string &actualCaseDescription);

	bool match (const string& output);
		// Regular Expression compilation (with flags in mind) and comparison with the input and output evaluation

	string studentOutputExpected();
		// Returns the expression without flags nor '/'

	OutputChecker* clone();

	static bool typeMatch(const string& text);
		// Tests if it's a regular expression. A regular expressions should be between /../

	string type();
};



/**
 * Class TestCase Declaration
 * TestCase represents cases of test
 */
class TestCase {
	static const char *command;
	static const char **argv;
	static const char **envv;
	int id;
	bool correctOutput;
	bool outputTooLarge;
	bool programTimeout;
	bool executionError;
	char executionErrorReason[1000];
	int sizeReaded;
	string input;
	vector< OutputChecker* > output;
	string caseDescription;
	float gradeReduction;
	float gradeReductionApplied;
	string programOutputBefore, programOutputAfter, programInput;

	void cutOutputTooLarge(string &output);
	void readWrite(int fdread, int fdwrite);
	void addOutput(const string &o, const string &actualCaseDescription);

public:
	vector<string> req;
	vector<string> depends;
	static std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>> requirements;
	static void setEnvironment(const char **environment);
	TestCase(const TestCase &o);
	TestCase& operator=(const TestCase &o);
	~TestCase();
	TestCase(int id, const string &input, const vector<string> &output, const vector<string> &req, const vector<string> &depends,
			const string &caseDescription, const float gradeReduction);
	bool isCorrectResult();
	float getGradeReduction();
	void setGradeReductionApplied(float r);
	float getGradeReductionApplied();
	string getCaseDescription() const;
	string getCommentTitle(bool withGradeReduction/*=false*/); // Suui
	string getComment();
	void runTest(time_t timeout);
	bool match(string data);
	int getOutputSize() const;
};



/**
 * Class Evaluation Declaration
 */
class Evaluation {
	int maxtime;
	float grademin, grademax;
	bool noGrade;
	float grade;
	int nerrors, nruns;
	vector<TestCase> testCases;
	char comments[MAXCOMMENTS + 1][MAXCOMMENTSLENGTH + 1];
	char titles[MAXCOMMENTS + 1][MAXCOMMENTSTITLELENGTH + 1];
	char titlesGR[MAXCOMMENTS + 1][MAXCOMMENTSTITLELENGTH + 1];
	volatile int ncomments;
	volatile bool stopping;
	static Evaluation *singlenton;
	Evaluation();

public:
	static Evaluation* getSinglenton();
	static void deleteSinglenton();
	void addTestCase(string &input, vector<string> &output, vector<string> &reqs, vector<string> &depends,
			string &caseDescription, float &gradeReduction);
	static void removeLastNL(string &s);
	static void removeLastWS(string &s);
	static void removeFirstWS(string &s);
	static void removeFirstNL(string &s);
	static void trim(string &s);
	bool cutToEndTag(string &value, const string &endTag);
	void loadTestCases(string fname);
	bool loadParams();
	void addFatalError(const char *m);
	void runTests();
	void outputEvaluation();
};



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// END OF DECLARATIONS ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// BEGINNING OF DEFINITIONS ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



volatile bool Stop::TERMRequested = false;
time_t Timer::startTime;
const char *TestCase::command = NULL;
const char **TestCase::argv = NULL;
const char **TestCase::envv = NULL;
Evaluation* Evaluation::singlenton = NULL;
void recursiveFindRequirementsAndDependencies(const std::vector<TestCase> &testCases, const TestCase &testCase, 
							   const std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>> &requirements, bool &allRequirementsPassed,
							   std::unordered_map<std::string, std::vector<std::string>> &errorMessages);
std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>> TestCase::requirements = std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>>{};



/**
 * Class Tools Definitions
 */

bool Tools::existFile(string name) {
	FILE *f = fopen(name.c_str(), "r");
	if (f != NULL) {
		fclose(f);
		return true;
	}
	return false;
}

string Tools::readFile(string name) {
	char buf[1000];
	string res;
	FILE *f = fopen(name.c_str(), "r");
	if (f != NULL)
		while (fgets(buf, 1000, f) != NULL)
			res += buf;
	return res;
}

vector<string> Tools::splitLines(const string &data) {
	vector<string> lines;
	int len, l = data.size();
	int startLine = 0;
	char pc = 0, c;
	for (int i = 0; i < l; i++) {
		c = data[i];
		if (c == '\n') {
			len = i - startLine;
			if (pc == '\r')
				len--;
			lines.push_back(data.substr(startLine, len));
			startLine = i + 1;
		}
		pc = c;
	}
	if (startLine < l) {
		len = l - startLine;
		if (pc == '\r')
			len--;
		lines.push_back(data.substr(startLine, len));
	}
	return lines;
}

int Tools::nextLine(const string &data) {
	int l = data.size();
	for (int i = 0; i < l; i++) {
		if (data[i] == '\n')
			return i + 1;
	}
	return l;
}

string Tools::caseFormat(string text) {
	vector<string> lines = Tools::splitLines(text);
	string res;
	int nlines = lines.size();
	for (int i = 0; i < nlines; i++)
		res += ">" + lines[i] + '\n';
	return res;
}

string Tools::toLower(const string &text) {
	string res = text;
	int len = res.size();
	for (int i = 0; i < len; i++)
		res[i] = tolower(res[i]);
	return res;
}

string Tools::normalizeTag(const string &text) {
	string res;
	int len = text.size();
	for (int i = 0; i < len; i++) {
		char c = text[i];
		if (isalpha(c) || c == '=')
			res += tolower(c);
	}
	return res;
}

string Tools::trimRight(const string &text) {
	int len = text.size();
	int end = -1;
	for (int i = len - 1; i >= 0; i--) {
		if (!isspace(text[i])) {
			end = i;
			break;
		}
	}
	return text.substr(0, end + 1);
}

string Tools::trim(const string &text) {
	int len = text.size();
	int begin = len;
	int end = -1;
	for (int i = 0; i < len; i++) {
		char c = text[i];
		if (!isspace(c)) {
			begin = i;
			break;
		}
	}
	for (int i = len - 1; i >= 0; i--) {
		char c = text[i];
		if (!isspace(c)) {
			end = i;
			break;
		}
	}
	if (begin <= end)
		return text.substr(begin, (end - begin) + 1);
	return "";
}

void Tools::fdblock(int fd, bool set) {
	int flags;
	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		return;
	}
	if (set && (flags | O_NONBLOCK) == flags)
		flags ^= O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
}

bool Tools::convert2(const string& str, double &data){
	stringstream conv(str);
	conv >> data;
	return conv.eof();
}

bool Tools::convert2(const string& str, long int &data){
	stringstream conv(str);
	conv >> data;
	return conv.eof();
}



/**
 * Class Stop Definitions
 */

void Stop::setTERMRequested() {
	TERMRequested = true;
}

bool Stop::isTERMRequested() {
	return TERMRequested;
}



/**
 * Class Timer Definitions
 */

void Timer::start() {
	startTime = time(NULL);
}

int Timer::elapsedTime() {
	return time(NULL) - startTime;
}



/**
 * Class Stop Definitions
 */

void I18n::init(){

}

const char *I18n::get_string(const char *s){
	return " ";
}



/**
 * Class NumbersOutput Definitions
 */


// Struct Number
bool NumbersOutput::Number::set(const string& str){
	isInteger=Tools::convert2(str, integer);
	if(!isInteger){
		return Tools::convert2(str, cientific);
	}
	return true;
}

bool NumbersOutput::Number::operator==(const Number &o)const{
	if(isInteger)
		return o.isInteger && integer==o.integer;
	if(o.isInteger)
		return cientific!=0?fabs((cientific-o.integer)/cientific) < 0.0001:o.integer==0;
	else
		return cientific!=0?fabs((cientific-o.cientific)/cientific) < 0.0001:fabs(o.cientific)<0.0001;
}

bool NumbersOutput::Number::operator!=(const Number &o)const{
	return !((*this)==o);
}


bool NumbersOutput::isNum(char c){
	if(isdigit(c)) return true;
	return c=='+' || c=='-' || c=='.' || c=='e' || c=='E';
}

bool NumbersOutput::isNumStart(char c){
	if(isdigit(c)) return true;
	return c=='+' || c=='-' || c=='.';
}

bool NumbersOutput::calcStartWithAsterisk(){
	int l=text.size();
	for(int i=0; i<l; i++){
		char c=text[i];
		if(isspace(c)) continue;
		if(c=='*'){
			cleanText = text.substr(i+1,text.size()-(i+1));
			return true;
		}else{
			cleanText = text.substr(i,text.size()-i);
			return false;
		}
	}
	return false;
}

NumbersOutput::NumbersOutput(const string &text):OutputChecker(text){
	int l=text.size();
	string str;
	Number number;
	for(int i=0; i<l; i++){
		char c=text[i];
		if((isNum(c) && str.size()>0) || (isNumStart(c) && str.size()==0)){
			str+=c;
		}else if(str.size()>0){
			if(isNumStart(str[0]) && number.set(str)) numbers.push_back(number);
			str="";
		}
	}
	if(str.size()>0){
		if(isNumStart(str[0]) && number.set(str)) numbers.push_back(number);
	}
	startWithAsterisk=calcStartWithAsterisk();
}

string NumbersOutput::studentOutputExpected(){
	return cleanText;
}

bool NumbersOutput::operator==(const NumbersOutput& o)const{
	int l=numbers.size();
	if(o.numbers.size() < l) return false;
	int offset=0;
	if(startWithAsterisk)
		offset=o.numbers.size()-l;
	for(int i=0; i<l; i++)
		if(numbers[i] != o.numbers[offset+i])
			return false;
	return true;
}

bool NumbersOutput::match(const string& output){
	NumbersOutput temp(output);
	return operator==(temp);
}

OutputChecker* NumbersOutput::clone(){
	return new NumbersOutput(outputExpected());
}

bool NumbersOutput::typeMatch(const string& text){
	int l=text.size();
	string str;
	Number number;
	for(int i=0; i<l; i++){
		char c=text[i];
		//Skip espaces/CR/LF... and *
		if(!isspace(c) && c!='*'){
			str+=c;
		}else if(str.size()>0){
			if(!isNumStart(str[0])||
				!number.set(str)) return false;
			str="";
		}
	}
	if(str.size()>0){
		if(!isNumStart(str[0])||!number.set(str)) return false;
	}
	return true;
}

string NumbersOutput::type(){
	return "numbers";
}



/**
 * Class TextOutput Definitions
 */

bool TextOutput::isAlpha(char c){
	if(isalnum(c)) return true;
	return c<0;
}

TextOutput::TextOutput(const string &text):OutputChecker(text){
	int l=text.size();
	string token;
	for(int i=0; i<l; i++){
		char c=text[i];
		if(isAlpha(c)){
			token+=c;
		}else if(token.size()>0){
			tokens.push_back(Tools::toLower(token));
			token="";
		}
	}
	if(token.size()>0){
		tokens.push_back(Tools::toLower(token));
	}
}

bool TextOutput::operator==(const TextOutput& o){
	int l=tokens.size();
	if(o.tokens.size() < l) return false;
	int offset=o.tokens.size()-l;
	for(int i=0; i<l; i++)
		if(tokens[i] != o.tokens[offset+i])
			return false;
	return true;
}

bool TextOutput::match(const string& output){
	TextOutput temp(output);
	return operator==(temp);
}

OutputChecker* TextOutput::clone(){
	return new TextOutput(outputExpected());
}

bool TextOutput::typeMatch(const string& text){
	return true;
}

string TextOutput::type(){
	return "text";
}



/**
 * Class ExactTextOutput Definitions
 */

bool ExactTextOutput::isAlpha(char c){
	if(isalnum(c)) return true;
	return c<0;
}

ExactTextOutput::ExactTextOutput(const string &text):OutputChecker(text){
	string clean=Tools::trim(text);
	if(clean.size()>2 && clean[0]=='*'){
		startWithAsterix =true;
		cleanText=clean.substr(2,clean.size()-3);
	}else{
		startWithAsterix =false;
		cleanText=clean.substr(1,clean.size()-2);
	}
}

string ExactTextOutput::studentOutputExpected(){
	return cleanText;
}

bool ExactTextOutput::operator==(const ExactTextOutput& o){
	return match(o.text);
}

bool ExactTextOutput::match(const string& output){
	if(cleanText.size()==0 && output.size()==0) return true;
	string clean;
	//Clean output if text last char is alpha
	if(cleanText.size()>0 && isAlpha(cleanText[cleanText.size()-1])){
		clean=Tools::trimRight(output);
	}else{
		clean=output;
	}
	if(startWithAsterix){
		size_t start=clean.size()-cleanText.size();
		return cleanText.size()<=clean.size() &&
				cleanText == clean.substr(start,cleanText.size());
	}
	else
		return cleanText==clean;
}

OutputChecker* ExactTextOutput::clone(){
	return new ExactTextOutput(outputExpected());
}

bool ExactTextOutput::typeMatch(const string& text){
	string clean=Tools::trim(text);
	return (clean.size()>1 && clean[0]=='"' && clean[clean.size()-1]=='"')
			||(clean.size()>3 && clean[0]=='*' && clean[1]=='"' && clean[clean.size()-1]=='"');
}

string ExactTextOutput::type(){
	return "exact text";
}



/**
 * Class RegularExpressionOutput Definitions
 */

RegularExpressionOutput::RegularExpressionOutput(const string &text, const string &actualCaseDescription):OutputChecker(text) {

	errorCase = actualCaseDescription;
	int pos = 1;
	flagI = false;
	flagM = false;
	string clean = Tools::trim(text);

	while (clean[pos] != '/' && pos < clean.size()) {
		pos++;
	}
	cleanText = clean.substr(1,pos-1);
	if (pos + 1 != clean.size()) {
		pos = pos + 1;

		// Flag search
		while (pos < clean.size()) {

			switch (clean[pos]) {
				case 'i':
					flagI=true;
					break;
				case 'm':
					flagM=true;
					break;
				default:
					Evaluation* p_ErrorTest = Evaluation::getSinglenton();
					char wrongFlag = clean[pos];
					string flagCatch;
					stringstream ss;
					ss << wrongFlag;
					ss >> flagCatch;
					string errorType = string("Flag Error in case ")+ string(errorCase)+ string (", found a ") + string(flagCatch) + string (" used as a flag, only i and m available");
					const char* flagError = errorType.c_str();
					p_ErrorTest->addFatalError(flagError);
					p_ErrorTest->outputEvaluation();
					abort();
			}
			pos++;
		}
	}
}

// Regular Expression compilation (with flags in mind) and comparison with the input and output evaluation
bool RegularExpressionOutput::match (const string& output) {

	reti=-1;
	const char * in = cleanText.c_str();
	// Use POSIX-C regrex.h

	// Flag compilation
	if (flagI || flagM) {
		if (flagM && flagI) {
			reti = regcomp(&expression, in, REG_EXTENDED | REG_NEWLINE | REG_ICASE);
		} else if (flagM) {
			reti = regcomp(&expression, in, REG_EXTENDED | REG_NEWLINE);
		} else {
			reti = regcomp(&expression, in, REG_EXTENDED | REG_ICASE);
		}

	// No flag compilation
	} else {
		reti = regcomp(&expression, in, REG_EXTENDED);
	}

	if (reti == 0) { // Compilation was suscesful

		const char * out = output.c_str();
		reti = regexec(&expression, out, 0, NULL, 0);

		if (reti == 0) { // Match
			return true;
		} else if (reti == REG_NOMATCH){ // No match
			return false;

		} else { // Memory Error

			Evaluation* p_ErrorTest = Evaluation::getSinglenton();
			string errorType = string("Out of memory error, during maching case ") + string(errorCase);
			const char* flagError = errorType.c_str();
			p_ErrorTest->addFatalError(flagError);
			p_ErrorTest->outputEvaluation();
			abort();
		}

	} else { // Compilation error
		size_t length = regerror(reti, &expression, NULL, 0);
        char bff [length];
        (void) regerror(reti, &expression, bff, length);
		Evaluation* p_ErrorTest = Evaluation::getSinglenton();
		string errorType = string("Regular Expression compilation error")+string (" in case: ")+ string(errorCase) +string (".\n")+ string(bff);
		const char* flagError = errorType.c_str();
		p_ErrorTest->addFatalError(flagError);
		p_ErrorTest->outputEvaluation();
		abort();
		return false;
	}
}

// Returns the expression without flags nor '/'
string RegularExpressionOutput::studentOutputExpected() {return cleanText;}

OutputChecker* RegularExpressionOutput::clone() {
	return new RegularExpressionOutput(outputExpected(), errorCase);
}

// Tests if it's a regular expression. A regular expressions should be between /../
bool RegularExpressionOutput::typeMatch(const string& text) {
	string clean=Tools::trim(text);
	if (clean.size() > 2 && clean[0] == '/') {
		for (int i = 1; i < clean.size(); i++) {
			if (clean[i] == '/') {
				return true;
			}
		}
	}
	return false;
}

string RegularExpressionOutput::type() {
	return "regular expression";
}



/**
 * Class TestCase Definitions
 * TestCase represents cases of test
 */

void TestCase::cutOutputTooLarge(string &output) {
	if (output.size() > MAXOUTPUT) {
		outputTooLarge = true;
		output.erase(0, output.size() - MAXOUTPUT);
	}
}

void TestCase::readWrite(int fdread, int fdwrite) {
	const int MAX = 1024* 10 ;
	//Buffer size to read
	const int POLLBAD = POLLERR | POLLHUP | POLLNVAL;
	const int POLLREAD = POLLIN | POLLPRI;
	//Poll to read from program
	struct pollfd devices[2];
	devices[0].fd = fdread;
	devices[1].fd = fdwrite;
	char buf[MAX];
	devices[0].events = POLLREAD;
	devices[1].events = POLLOUT;
	int res = poll(devices, programInput.size()>0?2:1, 0);
	if (res == -1) //Error
		return;
	if (res == 0) //Nothing to do
		return;
	if (devices[0].revents & POLLREAD) { //Read program output
		int readed = read(fdread, buf, MAX);
		if (readed > 0) {
			sizeReaded += readed;
			if (programInput.size() > 1) {
				programOutputBefore += string(buf, readed);
				cutOutputTooLarge(programOutputBefore);
			} else {
				programOutputAfter += string(buf, readed);
				cutOutputTooLarge(programOutputAfter);
			}
		}
	}
	if (programInput.size() > 0 && devices[1].revents & POLLOUT) { //Write to program
		int written = write(fdwrite, programInput.c_str(), Tools::nextLine(
				programInput));
		if (written > 0) {
			programInput.erase(0, written);
		}
		if(programInput.size()==0){
			close(fdwrite);
		}
	}
}

void TestCase::addOutput(const string &o, const string &actualCaseDescription){
// actualCaseDescripction, used to get current test name for Output recognition
	if(ExactTextOutput::typeMatch(o))
		this->output.push_back(new ExactTextOutput(o));
	else if (RegularExpressionOutput::typeMatch(o))
		this->output.push_back(new RegularExpressionOutput(o, actualCaseDescription));
	else if(NumbersOutput::typeMatch(o))
		this->output.push_back(new NumbersOutput(o));
	else

		this->output.push_back(new TextOutput(o));
}

void TestCase::setEnvironment(const char **environment) {
	command = "./vpl_test";
	argv = new const char*[2];
	argv[0] = command;
	argv[1] = NULL;
	envv = environment;
}

TestCase::TestCase(const TestCase &o) {
	id=o.id;
	correctOutput=o.correctOutput;
	outputTooLarge=o.outputTooLarge;
	programTimeout=o.programTimeout;
	executionError=o.executionError;
	strcpy(executionErrorReason,o.executionErrorReason);
	sizeReaded=o.sizeReaded;
	input=o.input;
	caseDescription=o.caseDescription;
	gradeReduction=o.gradeReduction;
	gradeReductionApplied=o.gradeReductionApplied;
	programOutputBefore=o.programOutputBefore;
	programOutputAfter=o.programOutputAfter;
	programInput=o.programInput;
	for(int i=0; i<o.output.size(); i++){
		output.push_back(o.output[i]->clone());
	}
	for(int i=0; i<o.req.size(); i++){
		req.push_back(o.req[i]);
	}
	for(int i=0; i<o.depends.size(); i++){
		depends.push_back(o.depends[i]);
	}
}

TestCase& TestCase::operator=(const TestCase &o) {
	id=o.id;
	correctOutput=o.correctOutput;
	outputTooLarge=o.outputTooLarge;
	programTimeout=o.programTimeout;
	executionError=o.executionError;
	strcpy(executionErrorReason,o.executionErrorReason);
	sizeReaded=o.sizeReaded;
	input=o.input;
	caseDescription=o.caseDescription;
	gradeReduction=o.gradeReduction;
	gradeReductionApplied=o.gradeReductionApplied;
	programOutputBefore=o.programOutputBefore;
	programOutputAfter=o.programOutputAfter;
	programInput=o.programInput;
	for(int i=0; i<output.size(); i++)
		delete output[i];
	output.clear();
	for(int i=0; i<o.output.size(); i++){
		output.push_back(o.output[i]->clone());
	}
	req.clear();
	for(int i=0; i<o.req.size(); i++){
		req.push_back(o.req[i]);
	}
	depends.clear();
	for(int i=0; i<o.depends.size(); i++){
		depends.push_back(o.depends[i]);
	}
	return *this;
}

TestCase::~TestCase() {
	for(int i=0; i<output.size();i++)
		delete output[i];
}

TestCase::TestCase(int id, const string &input, const vector<string> &output, const vector<string> &req, const vector<string> &depends,
		const string &caseDescription, const float gradeReduction) {
	this->id = id;
	this->input = input;
	for(int i=0;i<output.size(); i++){
		addOutput(output[i], caseDescription);
	}
	for(int i=0;i<req.size(); i++){
		this->req.push_back(req[i]);
	}
	for(int i=0;i<depends.size(); i++){
		this->depends.push_back(depends[i]);
	}
	this->caseDescription = caseDescription;
	this->gradeReduction = gradeReduction;
	outputTooLarge = false;
	programTimeout = false;
	executionError = false;
	correctOutput = false;
	sizeReaded = 0;
	gradeReductionApplied =0;
	strcpy(executionErrorReason, "");
}

bool TestCase::isCorrectResult() {
	return correctOutput && !(programTimeout || outputTooLarge
			|| executionError);
}

float TestCase::getGradeReduction() {
	return gradeReduction;
}

void TestCase::setGradeReductionApplied(float r) {
	gradeReductionApplied=r;
}

float TestCase::getGradeReductionApplied() {
	return gradeReductionApplied;
}

string TestCase::getCaseDescription() const {
	return caseDescription;
}

string TestCase::getCommentTitle(bool withGradeReduction=false) {
	char buf[100];
	string ret;
	sprintf(buf, "Test %d", id);
	ret = buf;
	if (caseDescription.size() > 0) {
		ret += ": " + caseDescription;
	}
	if(withGradeReduction && getGradeReductionApplied()>0){
		sprintf(buf," (%.3f)", -getGradeReductionApplied());
		ret += buf;
	}
	ret += '\n';
	return ret;
}

string TestCase::getComment() {
	if (correctOutput && !(programTimeout || outputTooLarge
			|| executionError)) {
		return "";
	}
	char buf[100];
	string ret;
	if (programTimeout) {
		ret += "Program timeout\n";
	}
	if (outputTooLarge) {
		sprintf(buf, "Program output too large (%dKb)\n", sizeReaded / 1024);
		ret += buf;
	}
	if (executionError) {
		ret += executionErrorReason + string("\n");
	}
	if (!correctOutput) {
		if(output.size() > 0) {
			ret += "Incorrect program result\n";
			ret += " --- Input ---\n";
			ret += Tools::caseFormat(input);
			ret += "\n --- Program output ---\n";
			ret += Tools::caseFormat(programOutputBefore + programOutputAfter);
			ret += "\n --- Expected output ("+output[0]->type()+")---\n";
			ret += Tools::caseFormat(output[0]->studentOutputExpected());
		}
	}
	return ret;
}

void TestCase::runTest(time_t timeout) {//timeout in seconds
	time_t start = time(NULL);
	int pp1[2]; //Send data
	int pp2[2]; //Receive data
	if (pipe(pp1) == -1 || pipe(pp2) == -1) {
		executionError = true;
		sprintf(executionErrorReason, "Internal error: pipe error (%s)",
				strerror(errno));
		return;
	}
	pid_t pid;
	if ((pid = fork()) == 0) {
		//Execute
		close(pp1[1]);
		dup2(pp1[0], STDIN_FILENO);
		close(pp2[0]);
		dup2(pp2[1], STDOUT_FILENO);
		dup2(STDOUT_FILENO, STDERR_FILENO);
		setpgrp();
		execve(command, (char * const *) argv, (char * const *) envv);
		perror("Internal error, execve fails");
		abort(); //end of child
	}
	if (pid == -1) {
		executionError = true;
		sprintf(executionErrorReason, "Internal error: fork error (%s)",
				strerror(errno));
		return;
	}
	close(pp1[0]);
	close(pp2[1]);
	int fdwrite = pp1[1];
	int fdread = pp2[0];
	Tools::fdblock(fdwrite, false);
	Tools::fdblock(fdread, false);
	programInput = input;
	if(programInput.size()==0){ //No input
		close(fdwrite);
	}
	programOutputBefore = "";
	programOutputAfter = "";
	pid_t pidr;
	int status;
	while ((pidr = waitpid(pid, &status, WNOHANG | WUNTRACED)) == 0) {
		readWrite(fdread, fdwrite);
		usleep(5000);
		//TERMSIG or timeout or program output too large?
		if (Stop::isTERMRequested() || (time(NULL) - start) >= timeout
				|| outputTooLarge) {
			if ((time(NULL) - start) >= timeout) {
				programTimeout = true;
			}
			kill(pid, SIGTERM); // Send SIGTERM nomral termination
			int otherstatus;
			usleep(5000);
			if (waitpid(pid, &otherstatus, WNOHANG | WUNTRACED) == pid) {
				break;
			}
			if (kill(pid, SIGQUIT) == 0) { //Kill
				break;
			}
		}
	}
	if (pidr == pid) {
		if (WIFSIGNALED(status)) {
			int signal = WTERMSIG(status);
			executionError = true;
			sprintf(executionErrorReason,
					"Program terminated due to \"%s\" (%d)\n", strsignal(
							signal), signal);
		} else if (WIFEXITED(status)) {
			//Nothing TODO
		} else {
			executionError = true;
			strcpy(executionErrorReason,
					"Program terminated but unknown reason.");
		}
	} else if (pidr != 0) {
		executionError = true;
		strcpy(executionErrorReason, "waitpid error");
	}
	readWrite(fdread, fdwrite);
	correctOutput = match(programOutputAfter) || match(programOutputBefore
			+ programOutputAfter);
}

bool TestCase::match(string data) {
	for (int i = 0; i < output.size(); i++)
		if (output[i]->match(data))
			return true;
	return false;
}

int TestCase::getOutputSize() const {
	return output.size();
}



/**
 * Class Evaluation Definitions
 */

Evaluation::Evaluation() {
	grade = 0;
	ncomments = 0;
	nerrors = 0;
	nruns = 0;
	noGrade = true;
}

Evaluation* Evaluation::getSinglenton() {
	if (singlenton == NULL) {
		singlenton = new Evaluation();
	}
	return singlenton; //Fix by Jan Derriks
}

void Evaluation::deleteSinglenton(){
	if (singlenton != NULL) {
		delete singlenton;
		singlenton = NULL;
	}
}

void Evaluation::addTestCase(string &input, vector<string> &output, vector<string> &reqs, vector<string> &depends,
		string &caseDescription, float &gradeReduction) {
	testCases.push_back(TestCase(testCases.size() + 1, input, output, reqs, depends,
			caseDescription, gradeReduction));
	
	input = "";
	output.resize(0);
	reqs.resize(0);
	depends.resize(0);
	caseDescription = "";
	gradeReduction = std::numeric_limits<float>::min();
}

void Evaluation::removeLastNL(string &s) {
	if (s.size() > 0 && s[s.size() - 1] == '\n') {
		s.resize(s.size() - 1);
	}
}

void Evaluation::removeLastWS(string &s) {
	if (s.size() > 0 && s[s.size() - 1] == ' ') {
		s.resize(s.size() - 1);
	}
}

void Evaluation::removeFirstNL(string &s) {
	if (s.size() > 0 && s[0] == '\n') {
		s.erase(s.begin());
	}
}

void Evaluation::removeFirstWS(string &s) {
	if (s.size() > 0 && s[0] == ' ') {
		s.erase(s.begin());
	}
}

void Evaluation::trim(string &s) {
	while (s[s.size() - 1] == '\n') {
		removeLastNL(s);
	}
	while (s[s.size() - 1] == ' ') {
		removeLastWS(s);
	}
	while (s[0] == ' ') {
		removeFirstWS(s);
	}
	while (s[0] == '\n') {
		removeFirstNL(s);
	}
}

bool Evaluation::cutToEndTag(string &value, const string &endTag) {
	size_t pos;
	if (endTag.size() && (pos = value.find(endTag)) != string::npos) {
		value.resize(pos);
		return true;
	}
	return false;
}

void Evaluation::loadTestCases(string fname) {
	if(!Tools::existFile(fname)) return;
	const char *CASE_TAG = "case=";
	const char *INPUT_TAG = "input=";
	const char *INPUT_END_TAG = "inputend=";
	const char *OUTPUT_TAG = "output=";
	const char *OUTPUT_END_TAG = "outputend=";
	const char *GRADEREDUCTION_TAG = "gradereduction=";
	const char *REQUIRES_TAG = "requires=";
	const char *DEPENDS_ON_TAG = "dependencies=";
	enum {
		regular, ininput, inoutput, inrequire, independ
	} state, newstate;
	bool inCase = false;
	vector<string> lines = Tools::splitLines(Tools::readFile(fname));
	string inputEnd = "";
	string outputEnd = "";
	string requireEnd = "";
	string dependEnd = "";
	string input = "";
	string output = "";
	string require = "";
	string depend = "";
	string caseDescription = "";
	string tag, value;

	float gradeReduction = std::numeric_limits<float>::min();
	/*must be changed from String
	 * to pair type (regexp o no) and string*/
	vector<string> outputs;
	vector<string> reqs;
	vector<string> depends;
	state = regular;
	int nlines = lines.size();
	for (int i = 0; i < nlines; i++) {
		string &line = lines[i];
		if (line.empty()) {
			continue;
		}
		size_t poseq;
		if ((poseq = line.find('=')) != string::npos) {
			tag = Tools::normalizeTag(line.substr(0, poseq + 1));
			value = line.substr(poseq + 1);
		} else {
			tag.clear();
		}
		if (state == ininput) {
			if (inputEnd.size()) { //Check for end of input
				size_t pos = line.find(inputEnd);
				if (pos == string::npos) {
					input += line + "\n";
				} else {
					cutToEndTag(line, inputEnd);
					input += line;
					state = regular;
					continue; //Next line
				}
			} else if (tag.size() && (tag == OUTPUT_TAG || tag
					== GRADEREDUCTION_TAG || tag == CASE_TAG || tag == REQUIRES_TAG || tag == DEPENDS_ON_TAG)) {//New valid tag
				state = regular;
				//Go on to process the current tag
			} else {
				input += line + "\n";
				continue; //Next line
			}
		} else if (state == inoutput) {
			if (outputEnd.size()) { //Check for end of output
				size_t pos = line.find(outputEnd);
				if (pos == string::npos) {
					output += line + "\n";
				} else {
					cutToEndTag(line, outputEnd);
					output += line;
					outputs.push_back(output);
					output = "";
					state = regular;
					continue; //Next line
				}
			} else if (tag.size() && (tag == INPUT_TAG || tag == OUTPUT_TAG
					|| tag == GRADEREDUCTION_TAG || tag == CASE_TAG || tag == REQUIRES_TAG || tag == DEPENDS_ON_TAG)) {//New valid tag
				removeLastNL(output);
				outputs.push_back(output);
				output = "";
				state = regular;
			} else {
				output += line + "\n";
				continue; //Next line
			}
		} else if (state == inrequire) {
			if (requireEnd.size()) { //Check for end of output
				size_t pos = line.find(requireEnd);
				if (pos == string::npos) {
					require += line + "\n";
				} else {
					cutToEndTag(line, requireEnd);
					require += line;
					reqs.push_back(require);
					require = "";
					state = regular;
					continue; //Next line
				}
			} else if (tag.size() && (tag == INPUT_TAG || tag == OUTPUT_TAG
					|| tag == GRADEREDUCTION_TAG || tag == CASE_TAG || tag == REQUIRES_TAG || tag == DEPENDS_ON_TAG)) {//New valid tag
				trim(require);
				reqs.push_back(require);
				require = "";
				state = regular;
			} else {
				require += line + "\n";
				continue; //Next line
			}
		} else if (state == independ) {
			if (dependEnd.size()) { //Check for end of output
				size_t pos = line.find(dependEnd);
				if (pos == string::npos) {
					depend += line + "\n";
				} else {
					cutToEndTag(line, dependEnd);
					depend += line;
					depends.push_back(depend);
					depend = "";
					state = regular;
					continue; //Next line
				}
			} else if (tag.size() && (tag == INPUT_TAG || tag == OUTPUT_TAG
					|| tag == GRADEREDUCTION_TAG || tag == CASE_TAG || tag == REQUIRES_TAG || tag == DEPENDS_ON_TAG)) {//New valid tag
				trim(depend);
				depends.push_back(depend);
				depend = "";
				state = regular;
			} else {
				depend += line + "\n";
				continue; //Next line
			}
		}
		if (state == regular && tag.size()) {
			if (tag == DEPENDS_ON_TAG) {
				inCase = true;
			    if (cutToEndTag(value, dependEnd)) {
					depend = value;
				} else {
					state = independ;
					depend = value;
				}
			} else if (tag == REQUIRES_TAG) {
				inCase = true;
			    if (cutToEndTag(value, requireEnd)) {
					require = value;
				} else {
					state = inrequire;
					require = value;
				}		
			} else if (tag == INPUT_TAG) {
				inCase = true;
				if (cutToEndTag(value, inputEnd)) {
					input = value;
				} else {
					state = ininput;
					input = value + '\n';
				}
			} else if (tag == OUTPUT_TAG) {
				inCase = true;
				if (cutToEndTag(value, outputEnd))
					outputs.push_back(value);
				else {
					state = inoutput;
					output = value + '\n';
				}
			} else if (tag == GRADEREDUCTION_TAG) {
				inCase = true;
				value=Tools::trim(value);
				//A percent value?
				if(value.size()>1 && value[value.size()-1]=='%'){
					float percent = atof(value.c_str());
					gradeReduction = (grademax-grademin)*percent/100;
				}else{
					gradeReduction = atof(value.c_str());
				}
			} else if (tag == INPUT_END_TAG) {
				inputEnd = Tools::trim(value);
			} else if (tag == OUTPUT_END_TAG) {
				outputEnd = Tools::trim(value);
			} else if (tag == CASE_TAG) {
				if (inCase) {
					addTestCase(input, outputs, reqs, depends, caseDescription,
							gradeReduction);
				}
				inCase = true;
				caseDescription = Tools::trim(value);
			}
		}
	}
	
	//TODO review
	if (state == inoutput) {
		removeLastNL(output);
		outputs.push_back(output);
	}
	if (state == inrequire) {
		trim(require);
		reqs.push_back(require);
	}
	if (state == independ) {
		trim(depend);
		depends.push_back(depend);
	}
	if (inCase) { //Last case => save current
		addTestCase(input, outputs, reqs, depends, caseDescription, gradeReduction);
	}
}

bool Evaluation::loadParams() {
	grademin= VPL_GRADEMIN;
	grademax = VPL_GRADEMAX;
	maxtime = VPL_MAXTIME;
	noGrade = grademin>=grademax;
	//printf("Min=%f max=%f time=%d\n",grademin,grademax,maxtime);
	return true;
}

void Evaluation::addFatalError(const char *m) {
	float reduction=grademax-grademin;
	if(ncomments>= MAXCOMMENTS)
		ncomments = MAXCOMMENTS-1;

	snprintf(titles[ncomments],MAXCOMMENTSTITLELENGTH,"%s",m);
	snprintf(titlesGR[ncomments],MAXCOMMENTSTITLELENGTH,"%s (%.2f)",m,reduction);
	strcpy(comments[ncomments],"");
	ncomments++;
	grade = grademin;
}

void Evaluation::runTests() {
	std::unordered_map<std::string, std::vector<std::string>> errorMessages;
	if (testCases.size() == 0) {
		return;
	}
	if (maxtime < 0) {
		addFatalError("Global timeout");
		return;
	}
	nerrors = 0;
	nruns = 0;
	grade = grademax;
	float defaultGradeReduction = (grademax - grademin) / testCases.size();
	int testsWithOutput = 0;
	for (int i = 0; i < testCases.size(); i++) {
	    if (testCases[i].getOutputSize() > 0) {
	        testsWithOutput++;   
	    }
	}
	int timeout = 0;
	if (testsWithOutput > 0) {
		timeout = maxtime / testsWithOutput;
	}
	for (int i = 0; i < testCases.size(); i++) {
		printf("Testing %d/%lu : %s\n",i+1,(unsigned long)testCases.size(),testCases[i].getCaseDescription().c_str());
		if ((testCases[i].getOutputSize() > 0 && timeout <= 1) || Timer::elapsedTime() >= maxtime) {
			grade = grademin;
			addFatalError("Global timeout");
			return;
		}
		if (maxtime - Timer::elapsedTime() < timeout) { //Try to run last case
			timeout = maxtime - Timer::elapsedTime();
		}
		
		bool allRequirementsPassed = true;
		bool singleDependsPassed = true;
		bool allDependsPassed = true;
		
		int error_number = 1;
		for (int j = 0; j < testCases[i].req.size(); j++) {
		    std::vector<std::string> failedReqs;
			bool requirementPassed = false;
			std::string currentReq = testCases[i].req[j];
			uint64_t orIndex = currentReq.find('|');
			while (orIndex != std::string::npos) {
				std::string parsedReq = currentReq.substr(0, orIndex);
				trim(parsedReq);
				requirementPassed |= TestCase::requirements[parsedReq].first;
				if (!requirementPassed) {
					failedReqs.push_back(parsedReq);
				}
				currentReq = currentReq.substr(orIndex + 1, currentReq.size());
				orIndex = currentReq.find('|');
			}
			trim(currentReq);
			requirementPassed |= TestCase::requirements[currentReq].first;
			allRequirementsPassed &= requirementPassed;
			if (!requirementPassed) {
			    failedReqs.push_back(currentReq);
			    for (std::string& failedReq : failedReqs) {
    				for (int k = 0; k < TestCase::requirements[failedReq].second.size(); k++) {
    					std::string str_error_number = std::to_string(error_number++) + std::string(") ");
    					errorMessages[testCases[i].getCaseDescription()].push_back(str_error_number + TestCase::requirements[failedReq].second[k]);
    				}
			    }
			}
			
		}
		for (const std::string &name_dependency : testCases[i].depends) {
			const TestCase &dependency = *std::find_if(testCases.begin(), testCases.end(), [&](const TestCase &tc) {
				return tc.getCaseDescription() == name_dependency;
			});
			recursiveFindRequirementsAndDependencies(testCases, dependency, TestCase::requirements, singleDependsPassed, errorMessages);
			if (!singleDependsPassed) {
				allDependsPassed = false;
				std::string str_error_number = std::to_string(error_number++) + std::string(") ");
				errorMessages[testCases[i].getCaseDescription()].push_back(str_error_number + std::string("Requirements for <") + name_dependency + std::string("> are not satisfied!\n"));
			}
			singleDependsPassed = true;
		}
		nruns++;
		if (testCases[i].getOutputSize() > 0 && allRequirementsPassed && allDependsPassed) {
			testCases[i].runTest(timeout);
		}
		if ((testCases[i].getOutputSize() > 0 && !testCases[i].isCorrectResult()) || !allRequirementsPassed || !allDependsPassed) {
			if (Stop::isTERMRequested())
				break;
			float gr = testCases[i].getGradeReduction();
			if (gr == std::numeric_limits<float>::min())
				testCases[i].setGradeReductionApplied(defaultGradeReduction);
			else
				testCases[i].setGradeReductionApplied(gr);
			grade -= testCases[i].getGradeReductionApplied();
			if(grade<grademin)
				grade=grademin;
			nerrors++;
			if(ncomments<MAXCOMMENTS){
				strncpy(titles[ncomments], testCases[i].getCommentTitle().c_str(),
						MAXCOMMENTSTITLELENGTH);
				strncpy(titlesGR[ncomments], testCases[i].getCommentTitle(true).c_str(),
						MAXCOMMENTSTITLELENGTH);
				strncpy(comments[ncomments], testCases[i].getComment().c_str(),
						MAXCOMMENTSLENGTH);
				if (!allRequirementsPassed || !allDependsPassed) {
					strncat(comments[ncomments], "Check the program description. Your program doesn't meet this requirements:\n\n",
									MAXCOMMENTSLENGTH);
				}
				for (const std::string &errorMessage : errorMessages[testCases[i].getCaseDescription()]) {
					strncat(comments[ncomments], errorMessage.c_str(),
								MAXCOMMENTSLENGTH);
					error_number++;
				}
				ncomments++;
			}
		}
	}
}

void Evaluation::outputEvaluation() {
	const char* stest[]={" test","tests"};
	if (testCases.size() > 0) {
		if (ncomments > 1) {
			printf("\n<|--\n");
			printf("-Failed tests\n");
			for (int i = 0; i < ncomments; i++) {
				printf("%s", titles[i]);
			}
			printf("--|>\n");
		}
		if (ncomments > 0) {
			printf("\n<|--\n");
			for (int i = 0; i < ncomments; i++) {
				printf("-%s", titlesGR[i]);
				printf("%s\n", comments[i]);
			}
			printf("--|>\n");
		}
		if (nruns > 0) {
			int passed=nruns-nerrors;
			printf("\n<|--\n");
			printf("-Summary of tests\n");
			printf(">+------------------------------+\n");
			printf(">| %2d %s run/%2d %s passed |\n",
					nruns, nruns==1?stest[0]:stest[1],
					passed, passed==1?stest[0]:stest[1]); //Taken from Dominique Thiebaut
			printf(">+------------------------------+\n");
			printf("\n--|>\n");
		}
		if(!noGrade){
			char buf[100];
			sprintf(buf, "%5.2f", grade);
			int len = strlen(buf);
			if (len > 3 && strcmp(buf + (len - 3), ".00") == 0)
				buf[len - 3] = 0;
			printf("\nGrade :=>>%s\n", buf);
		}
	} else {
		printf("<|--\n");
		printf("-No test case found\n");
		printf("--|>\n");
	}
	fflush(stdout);
}

void nullSignalCatcher(int n) {
	//printf("Signal %d\n",n);
}

void signalCatcher(int n) {
	//printf("Signal %d\n",n);
	if (Stop::isTERMRequested()) {
		Evaluation* obj = Evaluation::getSinglenton();
		obj->outputEvaluation();
		abort();
	}
	Evaluation *obj = Evaluation::getSinglenton();
	if (n == SIGTERM) {
		obj->addFatalError("Global test timeout (TERM signal received)");
	} else {
		obj->addFatalError("Internal test error");
		obj->outputEvaluation();
		Stop::setTERMRequested();
		abort();
	}
	alarm(1);
}

void setSignalsCatcher() {
	//Remove as signal controllers as possible
	for(int i=0;i<31; i++)
		signal(i, nullSignalCatcher);
	signal(SIGINT, signalCatcher);
	signal(SIGQUIT, signalCatcher);
	signal(SIGILL, signalCatcher);
	signal(SIGTRAP, signalCatcher);
	signal(SIGFPE, signalCatcher);
	signal(SIGSEGV, signalCatcher);
	signal(SIGALRM, signalCatcher);
	signal(SIGTERM, signalCatcher);
}

void recursiveFindRequirementsAndDependencies(const std::vector<TestCase> &testCases, const TestCase &testCase,
							   const std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>> &requirements, bool &allRequirementsPassed,
							   std::unordered_map<std::string, std::vector<std::string>> &errorMessages) {
	
	for (int i = 0; i < testCase.req.size(); i++) {
	    bool requirementPassed = false;
    	std::string currentReq = testCase.req[i];
		uint64_t orIndex = currentReq.find('|');
		while (orIndex != std::string::npos) {
			std::string parsedReq = currentReq.substr(0, orIndex);
			Evaluation::trim(parsedReq);
			requirementPassed |= TestCase::requirements[parsedReq].first;
			currentReq = currentReq.substr(orIndex + 1, currentReq.size());
			orIndex = currentReq.find('|');
		}
		Evaluation::trim(currentReq);
		requirementPassed |= TestCase::requirements[currentReq].first;
		allRequirementsPassed &= requirementPassed;
	}
	
	for (int i = 0; i < testCase.depends.size(); i++) {
		const TestCase &dependency = *std::find_if(testCases.begin(), testCases.end(), [&](const TestCase &tc) {
			return tc.getCaseDescription() == testCase.depends[i];
		});
		recursiveFindRequirementsAndDependencies(testCases, dependency, requirements, allRequirementsPassed, errorMessages);
	}
	
}

int main(int argc, char *argv[], const char **env) {
	RUN_ALL_TESTS();
	Timer::start();
	TestCase::setEnvironment(env);
	setSignalsCatcher();
	Evaluation* obj = Evaluation::getSinglenton();	
	obj->loadParams();
	obj->loadTestCases(TESTCASEFILE);
	obj->runTests();
	obj->outputEvaluation();
	return EXIT_SUCCESS;
}

