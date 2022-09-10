#!/usr/bin/env python3

from todolist import ToDoList
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Add ToDo.')
    parser.add_argument('-f', '--filename', type=str, required=True, help='Filename of ToDo list.')

    args = parser.parse_args()
    todos = ToDoList(args.filename)
    for todo in todos.all():
            print("%s\t%s" % (todo.title, todo.description))
