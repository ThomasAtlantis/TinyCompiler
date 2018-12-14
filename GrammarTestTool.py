# -*- coding:utf-8
import random


def load_grammar(filename):
	grammar = {}
	start = ""
	with open(filename, 'r', encoding='utf-8') as data_file:
		for line in data_file.readlines():
			production = line.strip().split(' -> ')
			if len(production) < 2:
				continue
			if not start:
				start = production[0]
			if production[0] not in grammar.keys():
				grammar[production[0]] = []
			for right in production[1].split(' | '):
				grammar[production[0]].append(right.split())
	return start, grammar


def generate(symbol, grammar):
	global programs
	if symbol not in grammar.keys():
		if symbol != 'null':
			# if symbol == '@C':
			# 	programs += str(random.randint(0, 10))
			# elif symbol == '@I':
			# 	programs += chr(ord('a') + random.randint(0, 26))
			# else:
			programs += symbol
			if symbol in ['{', '}', ';']:
				programs += '\n'
			else:
				programs += ' '
		return
	right = random.sample(grammar[symbol], 1)[0]
	for s in right:
		generate(s, grammar)


if __name__ == "__main__":
	in_file = "c--_grammar_simp.txt"
	out_file = "c--_grammar_prog.txt"
	programs = ""
	indent = 0
	programs_set = set()
	start, grammar = load_grammar(in_file)
	for i in range(200):
		try:
			generate(start, grammar)
		except RuntimeError as e:
			programs = ""
			continue
		programs_set.add(programs)
		programs = ""
	with open(out_file, 'w', encoding='utf-8') as text:
		index = 0
		for p in programs_set:
			text.write('#' * 15 + ' ' + str(index) + ' ' + '#' * 15 + '\n')
			text.write(p)
			index += 1


