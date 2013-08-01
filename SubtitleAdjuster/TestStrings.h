//
//  Created by Simon Racz
//

#ifndef TESTSTRINGS_H
#define TESTSTRINGS_H

// Xcode overwrites \r\n line endings to \n, even inside R"()";'s
// so the strings are not proper SubRip snippets, hence the tests will fail on them.

string engIn =
R"(1
00:00:12,440 --> 00:00:16,433
No, it's too risky.
I'm through doing that shit,

2
00:00:16,560 --> 00:00:21,554
You always say that: `'I'm through,
never again, too dangerous,.,"

3
00:00:21,680 --> 00:00:25,719
- I'm always right too.
- But you forget about it in a day,

4
00:00:25,840 --> 00:00:30,994
- The days of me remembering have
just begun, - You sound Iike.,,

5
00:00:31,120 --> 00:00:36,114
- I sound like a sensible man. - No,
you sound like a duck. Quack, quack!)";

string engRes=
R"(1
00:00:13,440 --> 00:00:16,390
No, it's too risky.
I'm through doing that shit,

2
00:00:16,484 --> 00:00:20,174
You always say that: `'I'm through,
never again, too dangerous,.,"

3
00:00:20,267 --> 00:00:23,251
- I'm always right too.
- But you forget about it in a day,

4
00:00:23,340 --> 00:00:27,148
- The days of me remembering have
just begun, - You sound Iike.,,

5
00:00:27,241 --> 00:00:30,931
- I sound like a sensible man. - No,
you sound like a duck. Quack, quack!)
)";

string hunIn =
R"(1
00:00:19,200 --> 00:00:21,760
,,Soha tˆbbÈ, kock·zatos."

2
00:00:22,080 --> 00:00:25,600
- …s mindig igazam van.
- De azt·n elfelejted.

3
00:00:25,920 --> 00:00:29,120
Azelıtt elfelejtettem,
mostantÛl megjegyzem.

4
00:00:29,440 --> 00:00:31,680
Tudod, hogy beszÈlsz?

5
00:00:32,000 --> 00:00:35,200
- Mint egy Èpesz˚ ember.
- Mint egy kacsa.

6
00:00:36,480 --> 00:00:38,400
Tˆbbet nem mondom.

7
00:00:38,720 --> 00:00:41,600
Tˆbbet nem hallasz h·pogni.)";

string hunRes=
R"(1
00:00:17,646 --> 00:00:20,698
,,Soha tˆbbÈ, kock·zatos."

2
00:00:21,080 --> 00:00:25,277
- …s mindig igazam van.
- De azt·n elfelejted.

3
00:00:25,658 --> 00:00:29,474
Azelıtt elfelejtettem,
mostantÛl megjegyzem.

4
00:00:29,855 --> 00:00:32,526
Tudod, hogy beszÈlsz?

5
00:00:32,907 --> 00:00:36,723
- Mint egy Èpesz˚ ember.
- Mint egy kacsa.

6
00:00:38,249 --> 00:00:40,538
Tˆbbet nem mondom.

7
00:00:40,920 --> 00:00:44,354
Tˆbbet nem hallasz h·pogni.)
)";

#endif
