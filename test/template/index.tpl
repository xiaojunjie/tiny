<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<meta name="Author" content="XJJFLY">
	<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
	<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7, IE=9">
	<link rel="shortcut icon" href="/favicon.ico">
	<link rel="icon" href="/favicon.ico">
	<link rel="mask-icon" href="https://developer.apple.com/apple-logo.svg" color="#333333">
	<link rel="stylesheet" href="/css/globalnav.css" type="text/css">
	<link rel="stylesheet" href="/css/global.dist.css" type="text/css">

	<script src="/js/jquery-1.11.0.min.js"></script>
	<script src="/js/jquery.retinate.js"></script>
	<script src="/js/global.js"></script>
	<script src="/js/global-logout.js"></script>

	<link rel="stylesheet" href="/css/fonts1.css" type="text/css">
	<link rel="stylesheet" href="/css/fonts2.css" type="text/css">
	<link rel="stylesheet" href="/css/fonts3.css" type="text/css">
	<link rel="stylesheet" href="/css/fonts4.css" type="text/css">

	<title>Tiny Web Server - XJJFLY Developer</title>
	<meta name="omni_page" content="Swift - Apple Developer">
	<meta name="Description" content="Swift is a powerful and intuitive programming language for iOS, macOS, tvOS, and watchOS. Writing Swift code is interactive and fun, the syntax is concise yet expressive, and Swift includes modern features developers love. Swift code is safe by design, yet also produces software that runs lightning-fast.">
	<link rel="alternate" type="application/rss+xml" title="RSS" href="feed://developer.apple.com/swift/blog/news.rss">
	<link rel="shortcut icon" href="/favicon.ico">
	<link rel="icon" href="/favicon.ico">
	<link rel="mask-icon" href="https://developer.apple.com/swift/swift-logo.svg" color="#fd6e33">
	<link rel="apple-touch-icon" href="https://developer.apple.com/swift/images/swift-touch-icon.png">
	<meta property="og:title" content="Swift - Apple Developer">
	<meta property="og:url" content="https://developer.apple.com/swift/">
	<meta property="og:description" content="Swift is a powerful and intuitive programming language for iOS, macOS, tvOS, and watchOS. Writing Swift code is interactive and fun, the syntax is concise yet expressive, and Swift includes modern features developers love. Swift code is safe by design, yet also produces software that runs lightning-fast.">
	<meta property="og:image" content="https://developer.apple.com/swift/images/swift-og.png">
	<meta name="twitter:card" content="summary">
	<meta name="twitter:image" content="https://developer.apple.com/swift/images/swift-og.png">
	<meta name="twitter:site" content="@SwiftLang">
	<meta name="twitter:creator" content="@SwiftLang">
	<meta name="twitter:title" content="Swift - Apple Developer">
	<meta name="twitter:description" content="Swift is a powerful and intuitive programming language for iOS, macOS, tvOS, and watchOS. Writing Swift code is interactive and fun, the syntax is concise yet expressive, and Swift includes modern features developers love. Swift code is safe by design, yet also produces software that runs lightning-fast.">
	<link href="/css/swift.css" rel="stylesheet" type="text/css">
</head>

<body id="overview">

	<input type="checkbox" id="ac-gn-menustate" class="ac-gn-menustate">
	<div id="ac-gn-curtain" class="ac-gn-curtain"></div>
	<script type="text/javascript" src="/js/ac-globalnav.built.js"></script>
	<div id="ac-gn-viewport-emitter">&nbsp;</div>

	<div id="top">
		<!-- SiteCatalyst code version: H.8. Copyright 1997-2006 Omniture, Inc. -->
		<script type="text/javascript">
			/* RSID: */
			var s_account = "appleglobal,appleusdeveloper"
		</script>

		<script type="text/javascript" src="/js/s_code_h.js"></script>
		<script type="text/javascript">
			s.pageName = AC && AC.Tracking && AC.Tracking.pageName();
			s.channel = "www.us.developer"

			/************* DO NOT ALTER ANYTHING BELOW THIS LINE ! **************/
			var s_code = s.t();
			if (s_code) document.write(s_code)
		</script>
		<!-- End SiteCatalyst code version: H.8. -->
	</div>

	<link rel="stylesheet" property="stylesheet" href="/css/localnav.css" type="text/css">
	<input type="checkbox" id="localnav-menustate" class="localnav-menustate">
	<nav id="localnav" class="localnav localnav-scrim" data-sticky="" role="navigation">
		<div class="localnav-wrapper">
			<div class="localnav-background"></div>
			<div class="localnav-content">

				<h2 class="localnav-title">
				<a href="https://developer.apple.com/swift/" onclick="s_objectID=&quot;https://developer.apple.com/swift/_1&quot;;return this.s_oc?this.s_oc(e):true">Tiny</a>
			</h2>

				<div class="localnav-menu">
					<a href="https://developer.apple.com/swift/#localnav-menustate" class="localnav-menucta-anchor localnav-menucta-anchor-open" id="localnav-menustate-open" onclick="s_objectID=&quot;https://developer.apple.com/swift/#localnav-menustate_1&quot;;return this.s_oc?this.s_oc(e):true">
						<span class="localnav-menucta-anchor-label">Open Menu</span>
					</a>
					<a href="https://developer.apple.com/swift/#" class="localnav-menucta-anchor localnav-menucta-anchor-close" id="localnav-menustate-close" onclick="s_objectID=&quot;https://developer.apple.com/swift/#_2&quot;;return this.s_oc?this.s_oc(e):true">
						<span class="localnav-menucta-anchor-label">Close Menu</span>
					</a>
					<div class="localnav-menu-tray">
						<ul class="localnav-menu-items">
							<li class="localnav-menu-item">
								<a href="#introduce" class="localnav-menu-link" >简介</a>
							</li>
							<li class="localnav-menu-item">
								<a href="#features" class="localnav-menu-link" >特性</a>
							</li>
							<li class="localnav-menu-item">
								<a href="#usage" class="localnav-menu-link">使用</a>
							</li>
							<li class="localnav-menu-item">
								<a href="#playgrounds" class="localnav-menu-link">设计</a>
							</li>
						</ul>
					</div>
					<div class="localnav-actions localnav-actions">
						<div class="localnav-action localnav-action-menucta" aria-hidden="true">
							<label for="localnav-menustate" class="localnav-menucta">
							<span class="localnav-menucta-chevron"></span>
						</label>
						</div>
					</div>
				</div>

			</div>
		</div>
	</nav>
	<label id="localnav-curtain" for="localnav-menustate"></label>
	<script src="/js/ac-localnav.built.js"></script>


	<main id="main" class="main" role="main">

		<div class="grid">
			<div class="row">
				<div class="column gutter large-10 large-centered small-12 padding-bottom-small padding-top text-center">
					<img src="/img/swift-64x64.png" width="64" height="64" alt="">
					<h1 class="headline"><span class="title">Tiny</span> 1.0</h1>
					<h3 class="sub-headline">一个小型Web服务器，简单轻巧。</h3>
					<p class="intro"> Tiny是一个用C++编写的小型Web服务器，不依赖于任何第三库。Tiny支持最基本的Web服务器功能，如路由配置、模板渲染、日志记录等，并且拥有能够动态伸缩的线程池，能满足一定的并发需求。</p>
				</div>
			</div>
		</div>

		<figure class="hero-swift-main"></figure>

		<div class="grid" id="introduce">
			<div class="row padding-top padding-bottom">
				<div class="column gutter large-12 padding-top-small padding-bottom-small text-center">
					<h2><span class="nowrap">Tiny 1.0</span></h2>
					<p>Swift 4 builds on the strengths of Swift 3, delivering greater robustness and stability, providing source code compatibility with Swift 3, making improvements to the standard library, and adding features like smart key paths and serialization, all
						while shortening build times and reducing the size of app binaries.</p>
				</div>
			</div>
		</div>

		<div class="bg-light section-playgrounds">
			<div class="grid">
				<div class="row padding-top padding-bottom">
					<div class="column gutter large-12 padding-top-small padding-bottom-small text-center">
						<h2>Learn Swift on iPad</h2>
						<p>In Swift Playgrounds you create small programs called “playgrounds” that instantly show the results of the code that you write. A single line of code can make amazing things happen. Interactive lessons teach key coding concepts, and additional challenges
							and templates encourage you to explore code in exciting new ways and to create something completely unique. There are even playgrounds that use Bluetooth to control robots, drones, and other hardware accessories. It’s easy to share your creations
							with friends, or record and post videos of your playgrounds in action.</p>
						<p><a class="more" href="https://developer.apple.com/swift-playgrounds/" onclick="s_objectID=&quot;https://developer.apple.com/swift-playgrounds/_1&quot;;return this.s_oc?this.s_oc(e):true">Learn about Swift Playgrounds for iPad</a></p>
						<figure class="hero-swift-playgrounds"></figure>
					</div>
				</div>
			</div>
		</div>

		<div class="grid">
			<div class="row padding-top padding-bottom">
				<div class="column gutter large-12 padding-top-small padding-bottom-small">
					<h3>开源</h3>
					<p>Swift 4 was developed in the open at Swift.org, with source code, a bug tracker, mailing lists, and regular development builds available for everyone. This broad community of developers, both inside Apple as well as hundreds of outside contributors,
						work together to make Swift even more amazing. Swift already supports all Apple platforms as well as Linux, with community members actively working to port to even more platforms. We’re excited to see more ways in which Swift makes software safer
						and faster, while also making programming more fun.</p>
					<p>To learn more about the open source Swift community, visit <a href="https://swift.org/getting-started/" class="external" target="new" onclick="s_objectID=&quot;https://swift.org/getting-started/_1&quot;;return this.s_oc?this.s_oc(e):true">Swift.org</a></p>
				</div>
			</div>
		</div>

		<div class="grid">
			<div class="row divider-top padding-top padding-bottom">
				<div class="column large-12 small-12 gutter padding-top-small padding-bottom-small">
					<h3>Source Compatibility Modes</h3>
					<p>Swift 3 set up the language for source-level stability and now Swift 4 delivers source compatibility as a feature moving forward. With Swift 4, you don’t have to modify any of your code to use the new version of the compiler. Instead you can start
						using the new Swift 4 compiler and migrate at your own pace, taking advantage of new Swift 4 features, one module at a time.</p>
					<p>You can use the new Swift 4 compiler with three different modes:</p>
					<ol>
						<li><strong>Swift 3</strong> mode is the default for existing code and will build source code that built with the Swift 3 compiler</li>
						<li><strong>Swift 4</strong> mode enables you to use new features and performance optimizations of Swift 4, some of which may require migration, making it easier to migrate than the previous transition from Swift 2.2 to Swift 3.</li>
						<li><strong>Mixed mode</strong> allows you to take advantage of interoperability between binaries built with the same compiler. This helps developers who have projects that mix packages written with Swift 3 with packages written with Swift 4, as long
							as all packages are built using the Swift 4 compiler. This allows developers to gradually migrate specific portions of their code to Swift 4 over time.</li>
					</ol>
				</div>
			</div>

			<div class="row divider-top padding-top padding-bottom" id="features">
				<div class="column large-12 small-12 gutter padding-top-small padding-bottom-small">
					<h3>新特性</h3>
					<ul class="large-9 small-12">
						<li>Faster, easier to use Strings that retain Unicode correctness and add support for creating, using and managing substrings</li>
						<li>Smart key paths for type-safe, efficient, extensible key value coding for Swift types</li>
						<li>Enhancements to creating and manipulating Dictionary and Set types</li>
						<li>Extends support of archival and serialization to struct and enum types and enables type-safety for serializing to external formats such as JSON and plist</li>
						<li>Enforced exclusive access to memory</li>
					</ul>
				</div>
			</div>

			<div id="usage" class="row divider-top divider-bottom padding-top padding-bottom">
				<div class="column large-12 small-12 gutter padding-top-small padding-bottom-small">
					<h3>使用</h3>
					<p>Swift is the result of the latest research on programming languages, combined with decades of experience building Apple platforms. Named parameters brought forward from Objective-C are expressed in a clean syntax that makes APIs in Swift even easier
						to read and maintain. Inferred types make code cleaner and less prone to mistakes, while modules eliminate headers and provide namespaces. Memory is managed automatically, and you don’t even need to type semi-colons. These forward-thinking concepts
						result in a language that is easy and fun to use.</p>
					<pre><code><span class="key">extension</span> <span class="title">String</span> {
	<span class="key">var</span> banana : <span class="title">String</span> {
		<span class="key">let</span> shortName = <span class="title">self</span>.<span class="method">dropFirst</span>()
		<span class="key">return</span> <span class="string">"</span>\<span class="string">(</span><span class="key">self</span><span class="string">)</span> \<span class="string">(</span><span class="key">self</span><span class="string">) Bo B</span>\<span class="string">(</span>shortName<span class="string">) Banana Fana Fo F</span>\<span class="string">(</span>shortName<span class="string">)"</span>
	<span></span>}
}

<span class="key">let</span> bananaName = <span class="string">"Jimmy"</span>.<span class="pointer">banana</span>		<span class="comment">// "Jimmy Jimmy Bo Bimmy Banana Fana Fo Fimmy"</span></code></pre>

					<p>Swift has many other features to make your code more expressive:</p>
					<div class="row">
						<div class="column large-6 small-12 gutter no-padding-top no-padding-bottom">
							<ul>
								<li>Closures unified with function pointers</li>
								<li>Tuples and multiple return values</li>
								<li>Generics</li>
								<li>Fast and concise iteration over a range or collection</li>
							</ul>
						</div>
						<div class="column large-6 small-12 gutter no-padding-top no-padding-bottom">
							<ul>
								<li>Structs that support methods, extensions, and protocols</li>
								<li>Functional programming patterns, e.g., map and filter</li>
								<li>Native error handling using <span class="keyword">try</span> / <span class="keyword">catch</span> / <span class="keyword">throw</span></li>
							</ul>
						</div>
					</div>
				</div>
			</div>
			<div class="row padding-top padding-bottom" id="playgrounds">
				<div class="column large-6 small-12 gutter padding-top-small padding-bottom-small">

					<h4>Playgrounds and REPL in Xcode</h4>
					<p>Much like Swift Playgrounds for iPad, playgrounds in Xcode make writing Swift code incredibly simple and fun. Type a line of code and the result appears immediately. You can then Quick Look the result from the side of your code, or pin that result
						directly below. The result view can display graphics, lists of results, or graphs of a value over time. You can open the Timeline Assistant to watch a complex view evolve and animate, great for experimenting with new UI code, or to play an animated
						SpriteKit scene as you code it. When you’ve perfected your code in the playground, simply move that code into your project.</p>

					<h4>Read-Eval-Print-Loop (REPL)</h4>
					<p>The LLDB debugging console in Xcode includes an interactive version of the Swift language built right in. Use Swift syntax to evaluate and interact with your running app, or write new code to see how it works in a script-like environment. Available
						from within the Xcode console or in Terminal.</p>

					<h4><strong>Package Manager</strong></h4>
					<p>Introduced in Swift 3, Swift Package Manager is a cross-platform tool for building Swift libraries and executables, making it easy to create and manage dependencies. Swift 4 adds new workflow features and a more complete API for the Swift Package
						Manager. It’s now easier to develop multiple packages in tandem before tagging your first official release, to work on a branch of multiple packages together, and to make local changes to the source code of a dependency you don’t control. New API
						lets package authors define which products their packages make available to clients, and customize how their sources are organized on disk. And the overall API used to create a package is now cleaner and clearer, while retaining source-compatibility
						with older packages.</p>

					<h4>Fast and Powerful</h4>
					<p>From its earliest conception, Swift was built to be fast. Using the incredibly high-performance LLVM compiler, Swift code is transformed into optimized native code that gets the most out of modern hardware. The syntax and standard library have also
						been tuned to make the most obvious way to write your code also perform the best.</p>
					<p>Swift is a successor to both the C and Objective-C languages. It includes low-level primitives such as types, flow control, and operators. It also provides object-oriented features such as classes, protocols, and generics, giving Cocoa and Cocoa
						Touch developers the performance and power they demand.</p>

				</div>
				<div class="column large-6 small-12 gutter pad-top-2-col padding-bottom-small">

					<h4>Designed for Safety</h4>
					<p>Swift eliminates entire classes of unsafe code. Variables are always initialized before use, arrays and integers are checked for overflow, and memory is managed automatically. Syntax is tuned to make it easy to define your intent — for example, simple
						three-character keywords define a variable ( <span class="keyword">var</span> ) or constant ( <span class="keyword">let</span> ).</p>
					<p>Another safety feature is that by default Swift objects can never be <span class="keyword">nil</span>. In fact, the Swift compiler will stop you from trying to make or use a <span class="keyword">nil</span> object with a compile-time error. This
						makes writing code much cleaner and safer, and prevents a huge category of runtime crashes in your apps. However, there are cases where <span class="keyword">nil</span> is valid and appropriate. For these situations Swift has an innovative feature
						known as <i>optionals</i>. An optional may contain nil, but Swift syntax forces you to safely deal with it using the <span class="keyword">?</span> syntax to indicate to the compiler you understand the behavior and will handle it safely.</p>

					<h4>Objective-C Interoperability</h4>
					<p>You can create an entirely new application with Swift today, or begin using Swift code to implement new features and functionality in your app. Swift code co-exists along side your existing Objective-C files in the same project, with full access
						to your Objective-C API, making it easy to adopt.</p>
					<p>To get started with Swift, download Xcode and follow the tutorials available on the <a href="https://developer.apple.com/swift/resources/" onclick="s_objectID=&quot;https://developer.apple.com/swift/resources/_2&quot;;return this.s_oc?this.s_oc(e):true">Resources</a>						tab.</p>

				</div>
			</div>
		</div>

	</main>

	<section id="globalfooter-wrapper">
		<footer id="globalfooter" role="contentinfo">
			<nav class="footer-breadory">
				<div id="directorynav" class="directorynav">
					<div id="dn-cola" class="column">
						<h3><a href="https://developer.apple.com/discover/" onclick="s_objectID=&quot;https://developer.apple.com/discover/_2&quot;;return this.s_oc?this.s_oc(e):true">Discover</a></h3>
						<ul>
							<li><a href="https://developer.apple.com/macos/" onclick="s_objectID=&quot;https://developer.apple.com/macos/_1&quot;;return this.s_oc?this.s_oc(e):true">macOS</a></li>
							<li><a href="https://developer.apple.com/ios/" onclick="s_objectID=&quot;https://developer.apple.com/ios/_1&quot;;return this.s_oc?this.s_oc(e):true">iOS</a></li>
							<li><a href="https://developer.apple.com/watchos/" onclick="s_objectID=&quot;https://developer.apple.com/watchos/_1&quot;;return this.s_oc?this.s_oc(e):true">watchOS</a></li>
							<li><a href="https://developer.apple.com/tvos/" onclick="s_objectID=&quot;https://developer.apple.com/tvos/_1&quot;;return this.s_oc?this.s_oc(e):true">tvOS</a></li>
							<li><a href="https://developer.apple.com/programs/" onclick="s_objectID=&quot;https://developer.apple.com/programs/_1&quot;;return this.s_oc?this.s_oc(e):true">Developer Program</a></li>
							<li><a href="https://developer.apple.com/enterprise/" onclick="s_objectID=&quot;https://developer.apple.com/enterprise/_1&quot;;return this.s_oc?this.s_oc(e):true">Enterprise</a></li>
							<li><a href="https://developer.apple.com/education/" onclick="s_objectID=&quot;https://developer.apple.com/education/_1&quot;;return this.s_oc?this.s_oc(e):true">Education</a></li>
						</ul>
					</div>
					<div id="dn-colb" class="column">
						<h3><a href="https://developer.apple.com/design/" onclick="s_objectID=&quot;https://developer.apple.com/design/_2&quot;;return this.s_oc?this.s_oc(e):true">Design</a></h3>
						<ul>
							<li><a href="https://developer.apple.com/design/resources/" onclick="s_objectID=&quot;https://developer.apple.com/design/resources/_1&quot;;return this.s_oc?this.s_oc(e):true">Resources</a></li>
							<li><a href="https://developer.apple.com/videos/design/" onclick="s_objectID=&quot;https://developer.apple.com/videos/design/_1&quot;;return this.s_oc?this.s_oc(e):true">Videos</a></li>
							<li><a href="https://developer.apple.com/design/awards/" onclick="s_objectID=&quot;https://developer.apple.com/design/awards/_1&quot;;return this.s_oc?this.s_oc(e):true">Apple Design Awards</a></li>
							<li><a href="https://developer.apple.com/accessibility/" onclick="s_objectID=&quot;https://developer.apple.com/accessibility/_1&quot;;return this.s_oc?this.s_oc(e):true">Accessibility</a></li>
							<li><a href="https://developer.apple.com/internationalization/" onclick="s_objectID=&quot;https://developer.apple.com/internationalization/_1&quot;;return this.s_oc?this.s_oc(e):true">Internationalization</a></li>
							<li><a href="https://developer.apple.com/accessories/" onclick="s_objectID=&quot;https://developer.apple.com/accessories/_1&quot;;return this.s_oc?this.s_oc(e):true">Accessories</a></li>
							<li><a href="https://developer.apple.com/app-store/marketing/guidelines/" onclick="s_objectID=&quot;https://developer.apple.com/app-store/marketing/guidelines/_1&quot;;return this.s_oc?this.s_oc(e):true">Marketing Guidelines</a></li>
						</ul>
					</div>
					<div id="dn-colc" class="column">
						<h3><a href="https://developer.apple.com/develop/" onclick="s_objectID=&quot;https://developer.apple.com/develop/_2&quot;;return this.s_oc?this.s_oc(e):true">Develop</a></h3>
						<ul>
							<li><a href="https://developer.apple.com/xcode/" onclick="s_objectID=&quot;https://developer.apple.com/xcode/_1&quot;;return this.s_oc?this.s_oc(e):true">Xcode</a></li>
							<li><a href="https://developer.apple.com/swift/" onclick="s_objectID=&quot;https://developer.apple.com/swift/_3&quot;;return this.s_oc?this.s_oc(e):true">Swift</a></li>
							<li><a href="https://developer.apple.com/documentation/" onclick="s_objectID=&quot;https://developer.apple.com/documentation/_1&quot;;return this.s_oc?this.s_oc(e):true">Documentation</a></li>
							<li><a href="https://developer.apple.com/videos/" onclick="s_objectID=&quot;https://developer.apple.com/videos/_1&quot;;return this.s_oc?this.s_oc(e):true">Videos</a></li>
							<li><a href="https://developer.apple.com/download/" onclick="s_objectID=&quot;https://developer.apple.com/download/_1&quot;;return this.s_oc?this.s_oc(e):true">Downloads</a></li>
						</ul>
					</div>
					<div id="dn-cold" class="column">
						<h3><a href="https://developer.apple.com/distribute/" onclick="s_objectID=&quot;https://developer.apple.com/distribute/_2&quot;;return this.s_oc?this.s_oc(e):true">Distribute</a></h3>
						<ul>
							<li><a href="https://developer.apple.com/app-store/" onclick="s_objectID=&quot;https://developer.apple.com/app-store/_1&quot;;return this.s_oc?this.s_oc(e):true">App Store</a></li>
							<li><a href="https://developer.apple.com/app-store/review/" onclick="s_objectID=&quot;https://developer.apple.com/app-store/review/_1&quot;;return this.s_oc?this.s_oc(e):true">App Review</a></li>
							<li><a href="https://itunesconnect.apple.com/" onclick="s_objectID=&quot;https://itunesconnect.apple.com/_1&quot;;return this.s_oc?this.s_oc(e):true">iTunes Connect</a></li>
							<li><a href="https://developer.apple.com/testflight/" onclick="s_objectID=&quot;https://developer.apple.com/testflight/_1&quot;;return this.s_oc?this.s_oc(e):true">TestFlight</a></li>
							<li><a href="https://developer.apple.com/enterprise/" onclick="s_objectID=&quot;https://developer.apple.com/enterprise/_2&quot;;return this.s_oc?this.s_oc(e):true">Enterprise</a></li>
							<li><a href="https://developer.apple.com/safari/extensions/" onclick="s_objectID=&quot;https://developer.apple.com/safari/extensions/_1&quot;;return this.s_oc?this.s_oc(e):true">Safari Extensions</a></li>
						</ul>
					</div>
					<div id="dn-cole" class="column">
						<h3><a href="https://developer.apple.com/support/" onclick="s_objectID=&quot;https://developer.apple.com/support/_2&quot;;return this.s_oc?this.s_oc(e):true">Support</a></h3>
						<ul>
							<li><a href="https://forums.developer.apple.com/" onclick="s_objectID=&quot;https://forums.developer.apple.com/_1&quot;;return this.s_oc?this.s_oc(e):true">Developer Forums</a></li>
							<li><a href="https://developer.apple.com/contact/" onclick="s_objectID=&quot;https://developer.apple.com/contact/_1&quot;;return this.s_oc?this.s_oc(e):true">Contact Us</a></li>
							<li><a href="https://developer.apple.com/bug-reporting/" onclick="s_objectID=&quot;https://developer.apple.com/bug-reporting/_1&quot;;return this.s_oc?this.s_oc(e):true">Bug Reporting</a></li>
							<li><a href="https://developer.apple.com/terms/" onclick="s_objectID=&quot;https://developer.apple.com/terms/_1&quot;;return this.s_oc?this.s_oc(e):true">License Agreements</a></li>
							<li><a href="https://developer.apple.com/system-status/" onclick="s_objectID=&quot;https://developer.apple.com/system-status/_1&quot;;return this.s_oc?this.s_oc(e):true">System Status</a></li>
						</ul>
					</div>
				</div>

			</nav>
			<div class="ac-gf-footer-legal">
				<div class="ac-gf-footer-news"> If you have any questions, please <a href="#">contact</a> me at any time.
					<div class="ac-gf-footer-legal-links">
					    <a href="http://www.xjjfly.com" class="first">Blog</a>
					    <a href="https://github.com/xiaojunjie">Github</a>
	  				</div>
				</div>
				<div class="ac-gf-footer-legal-copyright">Copyright © 2018 XJJFLY. All rights given up.</div>
			</div>

		</footer>
	</section>



</body>

</html>
