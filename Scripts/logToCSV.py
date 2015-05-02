import sys, traceback

def createCSV( filename ) :
	print "Starting " + filename
	r_file = open( filename , 'r')
	w_file = open( filename + '.csv', 'w')

	initial_band = []
	for line in r_file:
		if ( len(line) > 0 ) :
			initial_band.append( line[1:].split(",") )
		else :
			initial_band.append( [] );
	
	flows = {}
	counter = 0;

	for iter in initial_band:
		counter += 1
		for i in range(0, len(iter)-1) :
			if ( i%2 == 0 ) : 
				if ( iter[i] in flows.keys() ) :
					flows[iter[i]].append( [counter-1, int(iter[i+1])] );
				else :
					flows[iter[i]] = [[ counter-1, int(iter[i+1]) ]];
			
	band_by_flow = {}

	for flow, value in flows.iteritems() :
		f = [0] * len(initial_band)
		for band_track in value :
			f[ band_track[0] ] = band_track[1]
		
		band_by_flow[flow] = f[:]		
	
	w_file.write( "," + ",".join( map(str,range(1,len(initial_band)+1) )) + "\n" )
	for flow, bands in band_by_flow.iteritems() :
		w_file.write(flow + "," + ",".join( map( str, bands) ) + "\n");
		
	w_file.close()
	r_file.close()
	
def main():
    if (len(sys.argv[1:]) < 1) :
    	createCSV( "../hostBandwidth.log" )
    	sys.exit(0)
    	
    for filename in sys.argv[1:]:
    	createCSV( filename )
    	
    sys.exit(0)

if __name__ == "__main__":
    main()
