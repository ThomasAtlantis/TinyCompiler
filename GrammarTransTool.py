class SymbolGen:
	def __init__(self, order_ub):
		self.letter = ord('A')
		self.order = -1
		# upper bound of order: order_ub
		# letters_total: 26 * order_ub
		self.order_ub = order_ub

	def next(self):
		if self.order == self.order_ub - 1:
			if self.letter == ord('Z'):
				raise Exception("Fatal Error: symbol generator overflow")
			else:
				self.order = 0
				self.letter += 1
		else:
			self.order += 1
		return chr(self.letter) + str(self.order)


def word2symbol(word_dict, symbol_gen, word):
	if word[0] == "'" and len(word) > 1:
		return word[1:]
	if word[0] != '$':
		return word
	if word not in word_dict.keys():
		word_dict[word] = symbol_gen.next()
	return word_dict[word]


def grammar_trans(word_filename, symbol_filename, dict_filename):
	symbol_gen = SymbolGen(10)
	symbol_dict = {}
	with open(word_filename, "r", encoding="utf-8") as in_file:
		with open(symbol_filename, "w", encoding="utf-8") as out_file:
			for line in in_file.readlines():
				production = line.strip().split('->')
				if len(production) < 2:
					continue
				left_symbol = production[0].strip()
				right_symbols = production[1].strip().split()
				try:
					out_file.write('grammar.add("')
					out_file.write(word2symbol(symbol_dict, symbol_gen, left_symbol))
					out_file.write('") >> "' + word2symbol(symbol_dict, symbol_gen, right_symbols[0]))
					begin = False
					for symbol in right_symbols[1:]:
						if symbol == '|':
							out_file.write('" | "')
							begin = True
						else:
							if not begin:
								out_file.write(' ')
							else:
								begin = False
							out_file.write(word2symbol(symbol_dict, symbol_gen, symbol))
					out_file.write('"\n')
				except Exception as error:
					print(error)
					return
	with open(dict_filename, "w", encoding='utf-8') as dict_file:
		li = sorted(symbol_dict.items(), key=lambda x: x[1])
		for word, letter in li:
			dict_file.write(letter + " " + word + "\n")


if __name__ == '__main__':
	word_filename = "c--_grammar_raw.txt"
	symb_filename = "c--_grammar.txt"
	dict_filename = "c--_grammar_dict.txt"
	grammar_trans(word_filename, symb_filename, dict_filename)
	print('# 文法描述文件 #')
	with open(word_filename, encoding='utf-8') as text:
		print(text.read())
	print('# 文法程序源码 #')
	with open(symb_filename, encoding='utf-8') as text:
		print(text.read())
	print('# 文法符号映射 #')
	with open(dict_filename, encoding='utf-8') as text:
		print(text.read())

