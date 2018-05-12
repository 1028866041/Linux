package com.example.readinglist;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

@RunWith(SpringRunner.class)
@SpringBootApplicationConfiguration(
	class= ReadingListApplication.class)
@WebAppConfiguration
@SpringBootTest
public class DemoApplicationTests {

	@Test
	public void contextLoads() {
	}

}
