
Assignment: #2 - CMPUT 690 Data Mining in Rich Data 
Author : Shiva Zamani
Instructor: Osmar R. Zaiane
================================================PURPOSE:================================================

Implementation of the Apriori algorithm as published in the paper:
Rakesh Agrawal, Ramakrishnan Srikant
Fast Algorithms for Mining Association Rules
Proc. 20th Int. Conf. Very Large Data Bases, VLDB, 1994

================================================USAGE:================================================
	gcc Apriori.cpp -lstdc++ -std=c++11 -o ap
	./ap [INPUT FILE PATH] [MIN_SUPPORT] [MIN_CONFIDENCE] [DISPLAY_OPTION]

	sample usage:
	./ap dataT10K500D12L.data.txt 0.001 0.8 

Where:
	[INPUT FILE PATH]
		File name which include transactions.
		Each line in this file should represent a transaction. Items are separated by the space character. 
		This program Assumes that the items in each transaction are sorted.
		
	[MIN_SUPPORT]
		minimum support between 0 and 1.

	[MIN_CONFIDENCE]
		minimum confidence between 0 and 1.

	[DISPLAY_OPTION]
		r:
			display all strong association rules.
		f:
			display all frequent itemsets.
		a:
			display all frequent itemsets and all strong association rules.
		absent:
			display number of frequent k-itemsets and number of association rules.
