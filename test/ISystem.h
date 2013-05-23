/////////////////////////////////////////////////////
/// @file ISystem.h
///     An interface to operating system
///
/// @author Dmitry Kolzov
///
#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <memory>

struct Window;
class  ISystemListener;
class  IGraphicsContext;
class  IInput;

/////////////////////////////////////////////////////
///  OS API needed for the engine to operate
///  includes event handling (callback mechanism)
///  file creation, timings, etc
class ISystem
{ 
public:
    virtual ~ISystem() = 0;
    /// Add system events listener NEED TO FIX listeners lifetimes by introducing subscription objects probably
    virtual void AddListener(ISystemListener* systemListener) = 0;
    /// Create system independent graphics context
    virtual std::unique_ptr<IGraphicsContext> CreateGraphicsContext(Window const& window) = 0;
    /// Create system independent timer
    /// virtual std::unique_ptr<Timer> createTimer( std::function<void()> timerFunc ) = 0;
    /// Create input object
    virtual std::unique_ptr<IInput> CreateInput() = 0;
};

inline ISystem::~ISystem(){}

#endif // ISYSTEM_H