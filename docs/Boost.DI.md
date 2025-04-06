# Boost.DI Dependency Injection Tutorial

Boost.DI is a powerful, header-only C++14 dependency injection library that provides compile-time dependency injection with a clean, intuitive syntax. Let me explain it with a comprehensive example.

## Basic Concepts

Boost.DI allows you to:
- Define dependencies between components
- Automatically wire them together
- Manage object lifetimes
- Support different injection styles (constructor, field, etc.)

## Installation

First, include the Boost.DI header:
```cpp
#include <boost/di.hpp>
namespace di = boost::di;
```

## Full Example

Let's create a complete example with interfaces, implementations, and different types of dependencies.

### 1. Define Interfaces and Implementations

```cpp
// Logger interface
struct ILogger {
    virtual ~ILogger() = default;
    virtual void Log(const std::string& message) = 0;
};

// Console logger implementation
struct ConsoleLogger : ILogger {
    void Log(const std::string& message) override {
        std::cout << "Console: " << message << std::endl;
    }
};

// File logger implementation
struct FileLogger : ILogger {
    void Log(const std::string& message) override {
        std::cout << "File: " << message << std::endl;
        // (In real code, write to file here)
    }
};

// Engine interface
struct IEngine {
    virtual ~IEngine() = default;
    virtual void Start() = 0;
};

// Turbo engine implementation
struct TurboEngine : IEngine {
    TurboEngine(int horsepower) : horsepower_(horsepower) {}
    
    void Start() override {
        std::cout << "Turbo engine started with " << horsepower_ << " HP" << std::endl;
    }
    
private:
    int horsepower_;
};

// Car class that depends on logger and engine
class Car {
public:
    Car(std::shared_ptr<IEngine> engine, std::shared_ptr<ILogger> logger) 
        : engine_(engine), logger_(logger) {}
        
    void Drive() {
        engine_->Start();
        logger_->Log("Car is being driven");
    }
    
private:
    std::shared_ptr<IEngine> engine_;
    std::shared_ptr<ILogger> logger_;
};
```

### 2. Configure the Injector

```cpp
int main() {
    // Create an injector with bindings configuration
    auto injector = di::make_injector(
        // Bind ILogger interface to ConsoleLogger implementation
        di::bind<ILogger>().to<ConsoleLogger>(),
        
        // Bind IEngine interface to TurboEngine with parameter
        di::bind<IEngine>().to<TurboEngine>(),
        
        // Provide a value for the TurboEngine constructor parameter
        di::bind<int>().to(400) // horsepower
    );
    
    // Create a Car instance with all dependencies automatically resolved
    auto car = injector.create<std::shared_ptr<Car>>();
    
    // Use the car
    car->Drive();
    
    return 0;
}
```

### 3. Advanced Features

#### Named Parameters

```cpp
struct NamedExample {
    NamedExample(int speed, int torque) 
        : speed_(speed), torque_(torque) {}
        
    int speed_;
    int torque_;
};

auto injector = di::make_injector(
    di::bind<int>().named("speed"_s).to(120),
    di::bind<int>().named("torque"_s).to(250)
);

auto example = injector.create<NamedExample>();
std::cout << "Speed: " << example.speed_ << ", Torque: " << example.torque_ << std::endl;
```

#### Singleton vs. Unique Instances

```cpp
// Default is unique instance (new one created each time)
auto injector = di::make_injector(
    di::bind<ILogger>().to<ConsoleLogger>().in(di::unique), // default
    di::bind<IEngine>().to<TurboEngine>().in(di::singleton) // single instance
);
```

#### Multiple Interfaces to Same Implementation

```cpp
struct IFuelGauge {
    virtual float GetFuelLevel() = 0;
    virtual ~IFuelGauge() = default;
};

struct IOdometer {
    virtual float GetMileage() = 0;
    virtual ~IOdometer() = default;
};

class InstrumentCluster : public IFuelGauge, public IOdometer {
    // Implement both interfaces
};

auto injector = di::make_injector(
    di::bind<IFuelGauge, IOdometer>().to<InstrumentCluster>()
);
```

### 4. Practical Example with Multiple Components

```cpp
// Database interface
struct IDatabase {
    virtual void Save(const std::string& data) = 0;
    virtual ~IDatabase() = default;
};

// SQLDatabase implementation
class SQLDatabase : public IDatabase {
public:
    SQLDatabase(std::shared_ptr<ILogger> logger) : logger_(logger) {}
    
    void Save(const std::string& data) override {
        logger_->Log("Saving to SQL: " + data);
        // Actual database operations would go here
    }
    
private:
    std::shared_ptr<ILogger> logger_;
};

// UserService that depends on database and logger
class UserService {
public:
    UserService(std::shared_ptr<IDatabase> db, std::shared_ptr<ILogger> logger)
        : db_(db), logger_(logger) {}
        
    void CreateUser(const std::string& username) {
        logger_->Log("Creating user: " + username);
        db_->Save("User " + username);
    }
    
private:
    std::shared_ptr<IDatabase> db_;
    std::shared_ptr<ILogger> logger_;
};

int main() {
    auto injector = di::make_injector(
        di::bind<ILogger>().to<FileLogger>(),
        di::bind<IDatabase>().to<SQLDatabase>(),
        di::bind<IEngine>().to<TurboEngine>(),
        di::bind<int>().to(350) // horsepower
    );
    
    // Create services
    auto userService = injector.create<std::shared_ptr<UserService>>();
    userService->CreateUser("john_doe");
    
    auto car = injector.create<std::shared_ptr<Car>>();
    car->Drive();
    
    return 0;
}
```

## Key Benefits of Boost.DI

1. **Compile-time checking**: All dependencies are verified at compile time.
2. **Performance**: No runtime overhead compared to reflection-based DI.
3. **Flexibility**: Supports different lifetimes (singleton, unique, etc.).
4. **Clean code**: Promotes separation of concerns and testability.
5. **Minimal boilerplate**: No need for factory classes or manual wiring.

This example demonstrates the core features of Boost.DI. The library is highly customizable and can handle more complex scenarios as needed.

-----------------------------------------------------------------------------------------------

# Boost.DI C++ Tutorial: From Scratch to Advanced

## Introduction to Dependency Injection and Boost.DI

Boost.DI is a header-only C++14 dependency injection library that helps manage object dependencies in a clean, maintainable way. It implements the dependency injection pattern, where dependencies are "injected" into objects rather than being created internally.

### Why use Boost.DI?
- Reduces boilerplate code
- Makes dependencies explicit
- Improves testability
- Enhances modularity
- Simplifies complex object graph construction

## Getting Started

### Installation
Since Boost.DI is header-only, you can simply include it in your project:
```cpp
#include <boost/di.hpp>
namespace di = boost::di;
```

### Basic Example

```cpp
#include <boost/di.hpp>
#include <iostream>

namespace di = boost::di;

// Interfaces
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void Log(const std::string& message) = 0;
};

// Implementations
class ConsoleLogger : public ILogger {
public:
    void Log(const std::string& message) override {
        std::cout << "Console: " << message << std::endl;
    }
};

// Class with dependency
class UserService {
public:
    explicit UserService(std::shared_ptr<ILogger> logger) : logger_(logger) {}
    
    void CreateUser(const std::string& username) {
        logger_->Log("Creating user: " + username);
        // User creation logic...
    }

private:
    std::shared_ptr<ILogger> logger_;
};

int main() {
    // Create the injector
    auto injector = di::make_injector(
        di::bind<ILogger>().to<ConsoleLogger>()
    );
    
    // Get an instance of UserService with dependencies resolved
    auto userService = injector.create<std::shared_ptr<UserService>>();
    
    userService->CreateUser("john_doe");
    
    return 0;
}
```

## Core Concepts

### Bindings

Bindings tell Boost.DI how to resolve dependencies:

```cpp
auto injector = di::make_injector(
    di::bind<ILogger>().to<ConsoleLogger>(),  // Interface to implementation
    di::bind<ConsoleLogger>().in(di::singleton),  // Singleton scope
    di::bind<int>().to(42),  // Bind value
    di::bind<std::string>().to("Hello DI")  // Bind another value
);
```

### Scopes

Boost.DI supports different object lifetimes:

```cpp
auto injector = di::make_injector(
    di::bind<ILogger>().to<ConsoleLogger>().in(di::singleton),  // Single instance
    di::bind<IDatabase>().to<SqlDatabase>().in(di::unique)  // New instance each time (default)
);
```

### Constructor Injection

Boost.DI automatically resolves constructor parameters:

```cpp
class AuthService {
public:
    AuthService(std::shared_ptr<ILogger> logger, std::shared_ptr<IDatabase> db)
        : logger_(logger), db_(db) {}
    // ...
};
```

## Intermediate Topics

### Named Parameters

When you need to distinguish between parameters of the same type:

```cpp
struct db_server {};
struct db_port {};

auto injector = di::make_injector(
    di::bind<std::string>().named(db_server{}).to("localhost"),
    di::bind<int>().named(db_port{}).to(5432)
);

class DatabaseConfig {
public:
    DatabaseConfig(
        std::string server, 
        int port)
      : server_(server), port_(port) {}
    // ...
};
```

### Factory Providers

For complex object creation:

```cpp
class LoggerFactory {
public:
    template <class TInjector>
    auto operator()(const TInjector& injector) const {
        auto logger = injector.template create<std::shared_ptr<ConsoleLogger>>();
        logger->SetMinimumLevel(LogLevel::Debug);
        return logger;
    }
};

auto injector = di::make_injector(
    di::bind<ILogger>().to(LoggerFactory{})
);
```

### Polymorphic Objects

Handling derived classes:

```cpp
struct IProcessor { virtual ~IProcessor() = default; virtual void process() = 0; };
struct FastProcessor : IProcessor { void process() override { /*...*/ } };
struct SafeProcessor : IProcessor { void process() override { /*...*/ } };

auto injector = di::make_injector(
    di::bind<IProcessor>().to<FastProcessor>(),
    di::bind<IProcessor>().to<SafeProcessor>()  // Will override the previous binding
);
```

## Advanced Topics

### Configuration and Modules

Organizing bindings into modules:

```cpp
struct CoreModule {
    auto operator()() const {
        return di::make_injector(
            di::bind<ILogger>().to<ConsoleLogger>().in(di::singleton)
        );
    }
};

struct DataModule {
    auto operator()() const {
        return di::make_injector(
            di::bind<IDatabase>().to<SqlDatabase>()
        );
    }
};

auto injector = di::make_injector(
    CoreModule{},
    DataModule{}
);
```

### Custom Policies

Modifying how types are created:

```cpp
struct MyCreationPolicy {
    template <class T, class TMemory, class TInitializer, class... Args>
    auto operator()(const Args&... args) const {
        std::cout << "Creating instance of " << typeid(T).name() << std::endl;
        return new TMemory{args...};
    }
};

auto injector = di::make_injector<di::config<MyCreationPolicy>>(
    di::bind<ILogger>().to<ConsoleLogger>()
);
```

### Circular Dependencies

Handling circular references:

```cpp
class A;
class B;

class A {
public:
    explicit A(std::shared_ptr<B> b) : b_(b) {}
private:
    std::shared_ptr<B> b_;
};

class B {
public:
    explicit B(std::weak_ptr<A> a) : a_(a) {}
private:
    std::weak_ptr<A> a_;
};

auto injector = di::make_injector(
    di::bind<A>().in(di::singleton),
    di::bind<B>().in(di::singleton)
);
```

## Real-World Example

Here's a more complete example demonstrating several concepts:

```cpp
#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace di = boost::di;

// Interfaces
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void Log(const std::string& message) = 0;
};

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void Execute(const std::string& query) = 0;
};

// Implementations
class ConsoleLogger : public ILogger {
public:
    void Log(const std::string& message) override {
        std::cout << "LOG: " << message << std::endl;
    }
};

class SqlDatabase : public IDatabase {
public:
    SqlDatabase(const std::string& connectionString) 
        : connectionString_(connectionString) {}
    
    void Connect() override {
        std::cout << "Connecting to: " << connectionString_ << std::endl;
    }
    
    void Disconnect() override {
        std::cout << "Disconnecting from database" << std::endl;
    }
    
    void Execute(const std::string& query) override {
        std::cout << "Executing query: " << query << std::endl;
    }

private:
    std::string connectionString_;
};

// Services
class UserService {
public:
    UserService(std::shared_ptr<ILogger> logger, std::shared_ptr<IDatabase> db)
        : logger_(logger), db_(db) {
        logger_->Log("UserService created");
    }
    
    void RegisterUser(const std::string& username, const std::string& password) {
        logger_->Log("Registering user: " + username);
        db_->Connect();
        db_->Execute("INSERT INTO users VALUES ('" + username + "', '" + password + "')");
        db_->Disconnect();
    }

private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IDatabase> db_;
};

// Configuration
struct DbConfig {
    std::string connectionString = "server=localhost;database=mydb;user=admin";
};

auto ConfigureServices(const DbConfig& dbConfig) {
    return di::make_injector(
        di::bind<ILogger>().to<ConsoleLogger>().in(di::singleton),
        di::bind<IDatabase>().to<SqlDatabase>().in(di::unique),
        di::bind<std::string>().to(dbConfig.connectionString)
    );
}

int main() {
    DbConfig config;
    auto injector = ConfigureServices(config);
    
    auto userService = injector.create<std::shared_ptr<UserService>>();
    userService->RegisterUser("john_doe", "secure123");
    
    return 0;
}
```

## Testing with Boost.DI

One of the main benefits of DI is easier testing:

```cpp
#include <gtest/gtest.h>

// Mock Logger
class MockLogger : public ILogger {
public:
    MOCK_METHOD1(Log, void(const std::string&));
};

TEST(UserServiceTest, RegisterUserLogsCorrectly) {
    // Arrange
    auto mockLogger = std::make_shared<MockLogger>();
    auto mockDb = std::make_shared<MockDatabase>();
    
    EXPECT_CALL(*mockLogger, Log("Registering user: test_user")).Times(1);
    
    // Create injector with test doubles
    auto injector = di::make_injector(
        di::bind<ILogger>().to(mockLogger),
        di::bind<IDatabase>().to(mockDb)
    );
    
    auto userService = injector.create<std::shared_ptr<UserService>>();
    
    // Act
    userService->RegisterUser("test_user", "password");
    
    // Assert - Google Test macros verify expectations
}
```

## Performance Considerations

Boost.DI is designed to have minimal runtime overhead:
- Most work happens at compile-time
- No virtual calls for dependency resolution
- Can be optimized away by the compiler

However, for performance-critical code:
- Consider using singleton scope for expensive-to-create objects
- Be mindful of object graph complexity
- Profile if performance is a concern

## Best Practices

1. **Design to interfaces**: Depend on abstractions (interfaces) not concrete implementations
2. **Keep constructors simple**: They should just assign dependencies to members
3. **Use modules**: Organize related bindings together
4. **Prefer constructor injection**: Makes dependencies explicit
5. **Limit scope**: Use unique scope unless you specifically need singleton
6. **Avoid service locator pattern**: Explicitly declare dependencies instead

## Conclusion

Boost.DI provides a powerful, type-safe way to manage dependencies in C++ applications. By leveraging dependency injection, your code becomes more modular, testable, and maintainable. Start with simple injections and gradually adopt more advanced features as needed.

Remember that while DI has many benefits, it's not always necessary for small projects or simple object graphs. Evaluate whether the complexity is justified for your specific use case.