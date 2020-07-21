# Python Unit Tests

### Running tests

For running the python unit tests you will need to install first *testtools* which enables concurrent unit testing

```shell
pip3 install testtools
```

Then, you can run the unit tests from

```shell
./../unitTests.sh
```

### <u>Tests Structure</u>

#### odbUnitTest.py:

This includes `TestCase` class which inherits from `unittest.TestCase` with additional functionalities:

* `changeAndTest(self,obj,SetterName,GetterName,expectedVal,*args)`which is a function for changing a value and testing for the effect of that change where:

  * `obj` is the object to be tested
  * `SetterName` is the name of the function to be called for changing a value
  * `GetterName` is the name of the function to be called for testing the effect
  * `expectedVal` is the expected value for the testing
  * `*args` are the arguments passed to the `SetterName` function

  So, in the end, the expected behavior is:

  ```python
  obj.SetterName(*args)
  
  assert(obj.GetterName()==expectedVal)`
  ```

  

* `check(self,obj,GetterName,expectedVal,*args)` which tests against expected value

* `change(self,obj,SetterName,*args)` which changes a value in the object

* `main()` runs the `TestCase` in sequential order

* `mainParallel(Test)` runs the passed `Test` class in parallel

#### helper.py:

A set of functions for creating simple db instances to be used for testing.  You can find the description of each function in the comments

#### TestNet.py:

Unit test class for testing dbNet. It inherits from `odbUnitTest.TestCase` . it consists of

* `setUp(self)` function to be called before each test case. Here, we create the database with the desired chip, block, masters, instances and nets.
* `tearDown(self)` function to be called after each test case. Here, we destroy our db.
* `test_*(self)` the test cases functions. Their names should start with `test` for the unittest suite to recognize. 

#### TestDestroy.py:

Integration test class for testing the `destroy(*args)` function on OpenDB. it follows the same structure as TestNet.py





##### Problems Found In Testing

* multiple core dumps that leads to aborting the process:
  * dbNet.get1st*()			(when nothing on top of the list)
  * childRegion.getParent()       (after destroying the parent region)

