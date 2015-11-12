# check_ctags  

  This is a tool for checking redefined function names in multiple js files.    
Don't use a local var in the middle of a function.  
Use in the front of it, for Example,   
function xxx() {  
	var k;  
	...  
}  
instead of this:  
function xxx() {  
	...  
	var k;  
}  
