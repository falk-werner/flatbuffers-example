#!/usr/bin/env python3

from todolist import ToDoList
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Add ToDo.')
    parser.add_argument('-f', '--filename', type=str, required=True, help='Filename of ToDo list.')
    parser.add_argument('-t', '--title', type=str, required=True, help='Title of the new ToDo.')
    parser.add_argument('-d', '--description', type=str, default='', help='Description of the new ToDo')

    args = parser.parse_args()
    todos = ToDoList(args.filename)
    todos.add(args.title, args.description)
    todos.save(args.filename)