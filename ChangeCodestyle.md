![RayFoundation](https://raw.githubusercontent.com/kojiba/RayLanguage/master/foundation.logo.png "RayFoundation")
===========

#### Change code style:  
Ray is architect to be elastic enough for all developers.  
All code style dependencies stored in file RSyntax.  
Cause all based on define-code style you can create your own code style, you want.  

Main used style is camel case:  
```C
#define m(methodName, className) concatenate(methodName,    className) 
```  
Native function name like  
```C
    addObjectRArray(object, some)
```  
But if redefine calls section in RSyntax, to:  
```C
#define m(methodName, className) concatenate(concatenate(className, _), methodName) 
```  
you will get all methods native name like  
```C
    RArray_addObject(array, some_object)
```

Also if case-sensitive replace all 'RArray' in source files to 'r_array' you get pure linux-kernel naming style like:  
```C
    r_array_addObject(array, some_object)
```  

Please, use one your own selected code style for your code if you want to get ABI, API consistence. 