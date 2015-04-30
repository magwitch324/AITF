for i; do
	putty 10.4.13.$i -l cs4404 -pw AITF -m puttyfile
	psftp 10.4.13.$i -l cs4404 -pw AITF -b putfile
done
