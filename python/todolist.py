from collections import namedtuple

import flatbuffers
import fbs.todolist.ToDoList as FbsToDoList
import fbs.todolist.ToDo as FbsToDo

ToDo = namedtuple('ToDo', ['title', 'description'])

class ToDoList:
    def __init__(self, filename = None):
        self.todos = []
        if filename != None:
            try:
                with open(filename, 'rb') as f:
                    buffer = f.read()
                    root = FbsToDoList.ToDoList.GetRootAsToDoList(buffer)
                    for i in range(root.TodosLength()):
                        title = root.Todos(i).Title().decode('utf-8')
                        description = root.Todos(i).Description().decode('utf-8')
                        todo = ToDo(title, description)
                        self.todos.append(todo)
            except:
                None

    def add(self, title, description = ''):        
        todo = ToDo(title, description)
        self.todos.append(todo)
    
    def all(self):
        return self.todos

    def save(self, filename):
        builder = flatbuffers.Builder()

        titles = []
        descriptions = []
        todo_count = len(self.todos)
        for todo in self.todos:
            titles.append(builder.CreateString(todo.title))
            descriptions.append(builder.CreateString(todo.description))

        todos = []
        for i in range(0, todo_count):
            FbsToDo.ToDoStart(builder)
            FbsToDo.ToDoAddTitle(builder, titles[i])
            FbsToDo.ToDoAddDescription(builder, descriptions[i])
            todos.append(FbsToDo.ToDoEnd(builder))

        FbsToDoList.ToDoListStartTodosVector(builder, todo_count)
        for i in reversed(range(0, todo_count)):
            builder.PrependUOffsetTRelative(todos[i])
        fbs_todos = builder.EndVector()
        
        FbsToDoList.ToDoListStart(builder)
        FbsToDoList.ToDoListAddTodos(builder, fbs_todos)
        root = FbsToDoList.ToDoListEnd(builder)
        builder.Finish(root)
        buffer = builder.Output()
        with open(filename, 'wb') as f:
            f.write(buffer)
