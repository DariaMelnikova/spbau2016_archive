#! /usr/bin/env python3
# -*- codig: utf-8 -*-

#Не так давно мы писали скрипт, умеющий отслеживать, когда человек появляется онлайн в vk и когда исчезает. Теперь мы хотим научиться мониторить список друзей: кого человек добавляет, кого удаляет.
#Понятно, что с учетом существования скрытых друзей эту задачу полностью решить законным способом нельзя, зато вполне себе можно отслеживать всех открытых.
#Итак, нам нужно:
#1. Получать список идентификаторов друзей с помощью vk API.Для этого мы предлагаем использовать библиотеки requests и json.
#2. Далее из этого списка сформировать множество (set).
#3. Затем, в зависимости от параметров командной строки:
#• сохранить это множество на диске (с использованием pickle);
#• сравнить с уже имеющимся на диске множеством и вывести разницу: кто пришел, кто ушел.
#При запуске скрипт должен принимать параметром идентификатор пользователя и ключ, указывающий, что нужно сделать: -s или -save если нужно сохранить информацию на диск, и -d или --diff, если нужно показать разницу. Ключ и id могут идти в любом порядке.
#Например:
#./task3.py -s id1
#./task3.py id1 --diff
#Для обработки параметров рекомендуем использовать argparse.
#Если параметры указаны некорректно, нужно вывести справку и завершиться с ненулевым кодом возврата. Если по каким-то причинам не удалось выполнить работу (например, задан неверный id) нужно вывести сообщение об ошибке и также завершиться с ненулевым кодом возврата.
#Скриптом должно быть более-менее приятно пользоваться, так что выводить информацию нужно в пристойном виде: с именем того, за кем следим, с именами ушедших и пришедших друзей, а не одни идентификаторы.
#
#



import requests
import json
import pickle
import argparse


def __unicode__(self):
    return 'uni: %s' % self.name.decode('utf-8')


def get_id(s):
    if s[0] == 'i' and s[1] == 'd':
        return s[2:]
    else:
        print('Incorrect id')
        exit(1)


def get_name(id):
    r = requests.get(
            '''https://api.vk.com/method/users.get?
               PARAMETERS&user_id={}&fields=first_name,last_name
               &name_case=nom'''.format(id)).json()
    info = r['response'][0]
    return(info['first_name'] + " " + info['last_name'])


def create_new(args):
    try:
        id = get_id(args.id)
        r = requests.get('''https://api.vk.com/method/friends.get?
                            PARAMETERS&user_id={}&fields=uid,first_name,last_name
                            &name_case=nom'''.format(id)).json()
        set_names = set()
        for i in r["response"]:
            set_names.add(i["user_id"])
        with open('{}.pickle'.format(id), 'wb') as f:
            pickle.dump(set_names, f)
    except Exception:
        print("You are broke it all!!!")
        exit(1)


def diff(args):
    try:
        id = get_id(args.id)
        print('Search for: ' + get_name(id))
        r = requests.get('''https://api.vk.com/method/friends.get?
                            PARAMETERS&user_id={}&fields=uid,first_name,last_name
                            &name_case=nom'''.format(id)).json()
        new_set_names = set()
        for i in r["response"]:
            new_set_names.add(i["user_id"])
        with open('{}.pickle'.format(id), 'rb') as f:
            set_names = pickle.load(f)
        new = new_set_names.difference(set_names)
        print("New:")
        if len(new) != 0:
            for i in new:
                print(get_name(i) + " id" + id)
        else:
            print("None")
        deleted = set_names.difference(new_set_names)
        print("Deleted:")
        if len(deleted) != 0:
            for i in deleted:
                print(get_name(i) + " id" + id)
        else:
            print("None")
    except Exception:
        print("You are broke it all!!!")
        exit(1)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-s', '--save', help='type', action='store_true')
    parser.add_argument('-d', '--diff', help='type', action='store_true')
    parser.add_argument('id', help='user id', type=str)
    return parser.parse_args()


args = parse_args()
if args.save and args.diff:
    print("Incorrect input")
    exit(1)
if args.save:
    create_new(args)
if args.diff:
    diff(args)
