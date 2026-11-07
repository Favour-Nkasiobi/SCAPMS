#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <stack>
#include <string>
#include <functional>

namespace SCAPMS {

class UndoManager {
private:
    struct Operation {
        std::string description;
        std::function<void()> undoAction;
    };

    std::stack<Operation> operations;

public:
    void recordOperation(
        const std::string& description,
        std::function<void()> undoAction
        ) {
        operations.push({description, undoAction});
    }

    bool undoLastOperation() {
        if (operations.empty()) {
            return false;
        }

        Operation operation = operations.top();
        operations.pop();

        operation.undoAction();
        return true;
    }

    bool canUndo() const {
        return !operations.empty();
    }

    std::string lastOperationDescription() const {
        if (operations.empty()) {
            return "No operation to undo.";
        }

        return operations.top().description;
    }

    void clear() {
        while (!operations.empty()) {
            operations.pop();
        }
    }
};

}

#endif