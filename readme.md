SCROLL REVEAL
===================

A simple way to create and maintain how elements fade in, triggered when they enter the viewport. An open-source experiment from @JulianLloyd.

You may have seen this technique in the "single page" websites that are often step based marketing pages for an event, company services or a new product.

CONTENTS OF THIS FILE
---------------------

 - Introduction
 - Tested
 - Known Issues
 - Special Thanks
 - Requirements
 - Installation
 - Coming From Drupal?
 - Usage
 - License
 - Credits
 - Maintainers

TESTED
-----

This module has been manually tested successfully creating several working JQuery scroll items in Backdrop.

KNOWN ISSUES
---------------------

This module works as is by just installing it.

This module bundles a third-party Javascript scrollReveal.js from <https://github.com/julianlloyd/scrollReveal.js>
Copyright © 2015 Julian Lloyd
The MIT License (MIT)

Probably the preferred way to do this module is to use the Libraries module to bundle instead like this documentation:
<https://www.drupal.org/project/scrollreveal>

If it bothers you that we don't use the Libraries module, file an issue and we can try to change it.

When official information on using third-party JQuery plugins becomes known, this will follow it.

SPECIAL THANKS
--------------

Big thanks to Julian Lloyd for this functionality.

REQUIREMENTS
------------

A "parallax scrolling" looking website often has to use these four modules together:

- parallax_bg
- scrollreveal
- void_menu
- back_to_top

INSTALLATION
------------

Install this module using the official Backdrop CMS instructions at https://backdropcms.org/guide/modules


COMING FROM DRUPAL?
-------------------

Nothing substantially different.

PERMISSIONS
------------

@todo


USAGE
-----

Download and enable Scroll Reveal Module.

Go to admin page /admin/config/user-interface/scrollreveal and create the elements you wish fade when entering viewport.

Parameters

Element: Is a valid jQuery identification of the element that will receive the fade effect.

Enter: Controls the origin of the animation.

Over: Is the duration of the animation

After/Move: Is time before the reveal starts.

LICENSE
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

CREDITS
-----------

This module is based on the Scroll Reveal module for Drupal, originally written and maintained by a large number of contributors, including:

- hatuhay <https://www.drupal.org/u/hatuhay>

MAINTAINERS
-----------

- seeking

Ported to Backdrop by:

 - biolithic <https://github.com/biolithic>
