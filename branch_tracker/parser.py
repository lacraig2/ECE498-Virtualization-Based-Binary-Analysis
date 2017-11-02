class Memory:
	def __init__(instr):
		self.instr = instr
		self.mem = {}
	def add_addr(addr,mem):
		self.mem[addr]=mem

class MemoryTimeline:
	def __init__():
		self.timeline = {}
	def get_memory(instr):
		return self.timeline[instr]
	def add_memory(instr, memory):
		self.timeline[instr] = memory

def main():
	text = filter(lambda x: x.startswith("count") or x.startswith("addr"),open("../out.txt").readlines())
	timeline = MemoryTimeline()
	memory = None
	for line in text:
		if x.startswith("count"):
			memory = Memory(int(line.split()[1])
		else if s.startswith("addr"):
			stuff = line.split()
			memory.add_addr(int(stuff[1]), int(stuff[3]))



if __name__=="__main__":
	main()