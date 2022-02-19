/**
 * Unit Test Framework for compile time checking in VPL
 * @Copyright (C) 2020 Andrei-Edward Popa
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
 */
 
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <cstring>
#include <optional>

namespace utf {

class Assertion {
public:
	Assertion(bool check, const std::string &error) : mCheck(check) {
		mError = error + "\n";
	}

	bool check() const noexcept {
		return mCheck;
	}

	const std::string &error() const noexcept {
		return mError;
	}
private:
	bool mCheck{false};
	std::string mError{};
};

class AssertionHolder {
    typedef std::vector<Assertion>::iterator iterator;
    typedef std::vector<Assertion>::const_iterator const_iterator;
public:
	void insert(Assertion &&assertion) noexcept {
		mAssertions.push_back(std::move(assertion));
	}

	const Assertion &operator[](std::size_t index) const noexcept {
		return mAssertions[index];
	}

	std::size_t size() const noexcept {
		return std::size(mAssertions);
	}

	iterator begin() noexcept {
		return std::begin(mAssertions);
	}

	iterator end() noexcept {
		return std::end(mAssertions);
	}

	const_iterator cbegin() const noexcept {
		return std::cbegin(mAssertions);
	}

	const_iterator cend() const noexcept {
		return std::cend(mAssertions);
	}
private:
	std::vector<Assertion> mAssertions;
};

class Report {
public:
	bool satisfied() const noexcept {
		return mCheck;
	}

	const std::vector<std::string> &errors() const noexcept {
		return mErrors;
	}

	void insert(const Assertion &assertion) {
		mCheck &= assertion.check();
		if (!assertion.check()) {
			mErrors.push_back(assertion.error());
		}
	}
private:
	bool mCheck{true};
	std::vector<std::string> mErrors; 
};

class Test {
public:
	Test(const std::string &name, std::function<void(AssertionHolder &)> &&function) : mName(name), mFunction(std::move(function)) {}

	const std::string &name() const noexcept {
		return mName;
	}

	void run() noexcept {
		AssertionHolder holder;
		mFunction(holder);
		for (const Assertion &assertion : holder) {
			mReport.insert(assertion);
		}
	}

	const Report &report() const noexcept {
		return mReport;
	}
private:
	std::string mName;
	std::function<void(AssertionHolder &)> mFunction;
	Report mReport;
};

class Suite {
    typedef std::vector<Test>::iterator iterator;
    typedef std::vector<Test>::const_iterator const_iterator;
public:
	std::size_t size() const noexcept {
		return std::size(mTests);
	}

	iterator begin() noexcept {
		return std::begin(mTests);
	}

	iterator end() noexcept {
		return std::end(mTests);
	}

	const_iterator cbegin() const noexcept {
		return std::cbegin(mTests);
	}

	const_iterator cend() const noexcept {
		return std::cend(mTests);
	}

	void insert(const Test &test) noexcept {
		mTests.push_back(test);
	}

	template<typename ... Args>
	void emplace(Args&&... args) noexcept {
		mTests.emplace_back(std::forward<Args>(args)...);
	}

	void run() noexcept {
		for (Test &test : mTests) {
			test.run();
			mReports[test.name()] = test.report();
		}
	}

	const std::unordered_map<std::string, Report> &reports() const noexcept {
		return mReports;
	}
private:
	std::unordered_map<std::string, Report> mReports;
	std::vector<Test> mTests;
};

class SuiteHolder {
public:
	static SuiteHolder &Instance() noexcept {
		static SuiteHolder sInstance{};
		return sInstance;
	}

	void run_all() noexcept {
		for (auto &suite : mSuites) {
			auto &[suite_name, suite_tests] = suite;
			suite_tests.run();
			mReports[suite_name] = suite_tests.reports();
		}
	}

	void run_one(const std::string &suite_name, const std::string &test_name) noexcept {
		Test &test = *std::find_if(mSuites[suite_name].begin(), mSuites[suite_name].end(), [&](const Test &test) {
			return test.name() == test_name;
		});

		test.run();
		mReports[suite_name][test_name] = test.report();
	}

	void insert(const std::string &suite_name, const std::string &test_name, std::function<void(AssertionHolder &)> &&callback) {
		mSuites[suite_name].emplace(test_name, std::move(callback));
	}
	
	const Report &report(const std::string &suite_name, const std::string &test_name) const noexcept {
		return mReports.at(suite_name).at(test_name);
	}

	std::unordered_map<std::string, std::string> &segfault_messages() noexcept {
		return mSegmentationFaultMessages;
	}
private:
	SuiteHolder() = default;
	std::unordered_map<std::string, Suite> mSuites;
	std::unordered_map<std::string, std::unordered_map<std::string, Report>> mReports;
	std::unordered_map<std::string, std::string> mSegmentationFaultMessages;
};

class Insertion {
public:
	Insertion() = default;
	Insertion(const std::string& suite_name, const std::string& test_name, std::function<void(AssertionHolder &)> &&callback) {
		SuiteHolder::Instance().insert(suite_name, test_name, std::move(callback));
	}
};

template <typename... Args>
struct Ambiguate : public Args... {};

template<typename T, typename = void>
struct GotType : std::false_type {};

template<typename T>
struct GotType<T> : std::true_type {};

template<typename T, T>
struct SignatureCheck : std::true_type {};

template<typename Alias, typename AmbiguitySeed>
struct HasMember {
	template<typename C>
	static char ((&checker(decltype(&C::value))))[1];

	template<typename C>
	static char ((&checker(...)))[2];

	static_assert((sizeof(checker<AmbiguitySeed>(nullptr)) == 1), "Check AmbiguitySeed!");

	static constexpr bool value = sizeof(checker<Alias>(nullptr)) == 2;
};

template<typename, typename = void>
constexpr bool is_complete_type_v = false;

template<typename T>
constexpr bool is_complete_type_v<T, std::void_t<decltype(sizeof(T))>> = true;

template<typename T, typename Callable>
constexpr void call_if_class_defined(AssertionHolder &holder, Callable &&callable) {
	if constexpr (utf::is_complete_type_v<T>) {
		std::forward<Callable>(callable)(holder, static_cast<T*>(nullptr));
	}
}

template<typename Container>
std::optional<typename Container::value_type> get_container_item(Container container, uint64_t index) {
	auto it = std::next(container.begin(), index);
	if (std::distance(container.begin(), it) >= 0 && std::distance(it, container.end()) > 0) {
		return std::optional<typename Container::value_type>{*it};
	}
	return std::nullopt;
}

}
