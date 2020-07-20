package schoolApplication;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ AllModelTests.class, AllServiceTest.class })
public class AllTests {

}
