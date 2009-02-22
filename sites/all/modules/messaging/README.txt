// $Id: README.txt,v 1.1.4.1 2008/04/12 18:40:14 jareyero Exp $

README.txt - Drupal Module - Messaging
======================================

Drupal Messaging Framework

This is a messaging framework to allow message sending in a channel independent way
It will provide a common API for sending while allowing plugins for multiple channels

This Messaging Framework has been primarily developed to be used by the Notifications Framework.
See Drupal notifications module for an usage usage example implementing the full messaging capabilities.

See DEVELOPER.txt for technical and developers documentation. 

Features:
---------
- Provides a method agnostic API for composing and sending messages
- Handles message composition and formatting depending on sending method
- Supports multiple plug-ins for different message methods
- Supports 'push' and 'pull' message delivery

Plug-ins provided in this package:
---------------------------------
- messaging_mail: Integration with Drupal core mail API
- messaging_private: Integration with Privatemsg
- messaging_simple: Provides a simple UI for viewing pending messages for a user

Developers:
-----------
- Tim Cullen
- Jeff Miccolis
- Jose A. Reyero

Development Seed, http://www.developmentseed.org