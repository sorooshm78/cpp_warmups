# Dependency Injection in C++

Dependency Injection (DI) is a design pattern where an object receives its dependencies from an external source rather than creating them itself. This promotes loose coupling, easier testing, and more maintainable code.

## Basic Concepts

1. **Dependency**: An object that another object depends on (e.g., a service)
2. **Injection**: Providing the dependency to the dependent object (client) from outside

## Types of Dependency Injection

1. **Constructor Injection**: Dependencies are provided through the class constructor
2. **Setter Injection**: Dependencies are provided through setter methods
3. **Interface Injection**: Client implements an interface that the injector uses to provide dependencies

## Example: Constructor Injection

```cpp
#include <iostream>
#include <memory>
#include <string>

// Interface for the dependency
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

// Concrete implementation of the dependency
class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "Log: " << message << std::endl;
    }
};

// Another implementation (could be file logger, network logger, etc.)
class MockLogger : public ILogger {
public:
    void log(const std::string& message) override {
        // Does nothing - useful for testing
    }
};

// Client class that depends on ILogger
class Service {
private:
    std::unique_ptr<ILogger> logger_;
    
public:
    // Constructor injection - dependency is passed in
    explicit Service(std::unique_ptr<ILogger> logger) 
        : logger_(std::move(logger)) {}
    
    void doSomething() {
        logger_->log("Doing something...");
        // Actual work would go here
    }
};

int main() {
    // Create the dependency
    auto logger = std::make_unique<ConsoleLogger>();
    
    // Inject the dependency into the service
    Service service(std::move(logger));
    
    // Use the service
    service.doSomething();
    
    // For testing, we could inject a mock logger
    auto testLogger = std::make_unique<MockLogger>();
    Service testService(std::move(testLogger));
    testService.doSomething();  // Won't actually log anything
    
    return 0;
}
```

## Example: Setter Injection

```cpp
#include <iostream>
#include <memory>
#include <string>

// Same ILogger interface as before

class SetterInjectedService {
private:
    std::unique_ptr<ILogger> logger_;
    
public:
    // Default constructor - service can be created without logger
    SetterInjectedService() = default;
    
    // Setter for dependency injection
    void setLogger(std::unique_ptr<ILogger> logger) {
        logger_ = std::move(logger);
    }
    
    void doSomething() {
        if (logger_) {
            logger_->log("Setter injection doing something...");
        }
    }
};

int main() {
    SetterInjectedService service;
    
    // Inject dependency later
    service.setLogger(std::make_unique<ConsoleLogger>());
    service.doSomething();
    
    return 0;
}
```

## Benefits of Dependency Injection

1. **Testability**: Easier to test with mock dependencies
2. **Decoupling**: Reduces tight coupling between classes
3. **Flexibility**: Dependencies can be easily swapped
4. **Reusability**: Components are more reusable
5. **Maintainability**: Easier to modify and extend

## Modern C++ Considerations

- Use `std::unique_ptr` or `std::shared_ptr` for managing dependencies
- Prefer constructor injection for mandatory dependencies
- Consider using interfaces (abstract base classes) for dependencies
- For complex applications, consider DI containers/frameworks like:
  - Boost.DI
  - Fruit
  - Hypodermic

