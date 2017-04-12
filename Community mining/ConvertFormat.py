'''
File:	ConvertFormat.py
Author:	Shiva Zamani
Email:	zamanigh@ualberta.ca
Date:	12-April-2017
Description:
	Convert output format of DANCer to be compatible with input format needed for ILouvain
'''
 
import sys
import getopt
 
'''
ConvertFormat
Input:
	Path to inputfile
	format of input file should be as follow:
	Vertices section:
	the vertices section starts with the line # Vertices. 
	Each consecutive line describes a vertex. 
	A line consists of an integer corresponding to the vertex id, the list of its attribute values
	separated by a ; and an integer corresponding to the vertex community id. Each value is separated by a ;.
	This section ends with a # in a single line
	Edges section:
	the edges section starts with the line # Edges.
	Each consecutive line corresponds to an edge. A line is composed of two vertex ids separated by a ;.
	This format is the output format of Dancer
Output:
	3 files as follow:
	1- outputName.attributes
		Each line consists of an integer corresponding to the vertex id, the list of its attribute values separated by a ,
	2- outputName.community
		Each line consists of an integer corresponding to the vertex id, and an integer corresponding to the vertex community id
		separated by space
	3- outputName.edgeList
		Each line corresponds to an edge. A line is composed of two vertex ids separated by a space.
	This format is needed for ILouvain
'''
def  ConvertFormat(ifile,outputName):
	with open(ifile) as infile:
		graph = []
		edgeList= []
		edge=0
		for line in infile:
			line=line.rstrip('\n')
			if(line=="# Vertices"):
				continue
			if(line=="# Edges"):
				continue
			if(line=="#"):
				edge=1
				continue
			if(edge==1):
				edgeList.append(line)
			else:
				graph.append(line)	
	with open(outputName+".edgeList","w") as edgefile:
		for edge in edgeList:
			V1,V2=edge.split(";")
			edgefile.write(V1+" "+V2+"\n")
			
	with open(outputName+".attributes","w") as attrfile:
		for node in graph:
			VId,attrs,Community=node.split(";")
			attrList=attrs.split("|")
			
			attrfile.write(VId+ " ")
			
			attrSize=len(attrList)
			for i in range(attrSize):
				if(i!=len(attrList)-1):
					attrfile.write(attrList[i]+",")
				else:
					attrfile.write(attrList[i]+"\n")		
				
	with open(outputName+".community","w") as Communityfile:
		for node in graph:
			VId,attrs,Community=node.split(";")
			Communityfile.write(VId+" "+Community+"\n")
	
def main(argv):
	inputfile = ''
	outputName = ''
	try:
		opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
	except getopt.GetoptError:
		print('ConvertFormat.py -i <inputfile> -o <outputfile>')
		sys.exit(2)
	if len(sys.argv) <2:
		print('ConvertFormat.py -i <inputfile> -o <outputfile>')
		sys.exit(2)
	else:
		for opt, arg in opts:
			if opt == '-h':
				print('ConvertFormat.py -i <inputfile> -o <outputfile>')
				sys.exit()
			elif opt in ("-i", "--ifile"):
				inputfile = arg
			elif opt in ("-o", "--ofile"):
				outputName = arg
	ConvertFormat(inputfile,outputName)

if __name__ == "__main__":
	main(sys.argv[1:])

