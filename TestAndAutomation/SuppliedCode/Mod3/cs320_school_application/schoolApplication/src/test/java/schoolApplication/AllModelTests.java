package schoolApplication;

//import org.junit.internal.runners.TestClass;
import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;
import schoolApplication.model.TestStudent;
import schoolApplication.model.TestClass;

@RunWith(Suite.class)
@SuiteClasses({TestClass.class, TestStudent.class})
public class AllModelTests {

}
