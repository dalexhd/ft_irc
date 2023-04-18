/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 12:22:18 by aborboll          #+#    #+#             */
/*   Updated: 2023/04/18 16:12:15 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#define RPL_WELCOME(servername, nick) \
	":" + servername + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\n"
#define RPL_YOURHOST(servername, nick, version) \
	":" + servername + " 002 " + nick + " :Your host is " + servername + ", running version " + version + "\n"
#define RPL_CREATED(servername, nick) \
	":" + servername + " 003 " + nick + " :This server was created January 1st, 1970\n"
#define RPL_MYINFO(servername, nick, version, usermodes, chanmodes) \
	":" + servername + " 004 " + nick + " " + servername + " " + version + " " + usermodes + " " + chanmodes + " :" + servername + " server\n"
#define RPL_ISUPPORT(servername, nick, tokens) \
	":" + servername + " 005 " + nick + " " + tokens + " :are supported by this server\n"
#define RPL_BOUNCE 010
#define RPL_UMODEIS 221
#define RPL_LUSERCLIENT 251
#define RPL_LUSEROP 252
#define RPL_LUSERUNKNOWN 253
#define RPL_LUSERCHANNELS 254
#define RPL_LUSERME 255
#define RPL_ADMINME 256
#define RPL_ADMINLOC1 257
#define RPL_ADMINLOC2 258
#define RPL_ADMINEMAIL 259
#define RPL_TRYAGAIN 263
#define RPL_LOCALUSERS 265
#define RPL_GLOBALUSERS 266
#define RPL_WHOISCERTFP 276
#define RPL_NONE 300
#define RPL_AWAY 301
#define RPL_USERHOST 302
#define RPL_UNAWAY 305
#define RPL_NOWAWAY 306
#define RPL_WHOREPLY 352
#define RPL_ENDOFWHO 315
#define RPL_WHOISREGNICK 307
#define RPL_WHOISUSER(servername, nick, user, host, realname) \
	":" + servername + " 311 " + nick + " " + user + " " + host + " * :" + realname + "\n"
#define RPL_WHOISSERVER 312
#define RPL_WHOISOPERATOR 313
#define RPL_WHOWASUSER 314
#define RPL_WHOISIDLE 317
#define RPL_ENDOFWHOIS 318
#define RPL_WHOISCHANNELS(servername, nick, mode, channel) \
	":" + servername + " 311 " + nick + " " + mode + " #" + channel + "\n"
#define RPL_WHOISSPECIAL 320
#define RPL_LISTSTART(servername, nick) \
	":" + servername + " 321 " + nick + " Channel :Users Name\n"
#define RPL_LIST(servername, nick, channel, users, topic) \
	":" + servername + " 322 " + nick + " #" + channel + " " + users + " :" + topic + "\n"
#define RPL_LISTEND(servername, nick) \
	":" + servername + " 323 " + nick + " :End of /LIST\n"
#define RPL_CHANNELMODEIS(servername, nick, channel, modes) \
	":" + servername + " 324 " + nick + " #" + channel + " +" + modes + "\n"
#define RPL_CREATIONTIME 329
#define RPL_WHOISACCOUNT 330
#define RPL_NOTOPIC(servername, nick, channel) \
	":" + servername + " 331 " + nick + " #" + channel + " :No topic is set\n"
#define RPL_TOPIC(servername, nick, channel, topic) \
	":" + servername + " 332 " + nick + " #" + channel + " :" + topic + "\n"
#define RPL_TOPICWHOTIME(servername, nick, channel, user, time) \
	":" + servername + " 333 " + nick + " #" + channel + " " + user + " " + time + "\n"
#define RPL_WHOISACTUALLY 338
#define RPL_INVITING(servername, nick, channel, user) \
	":" + servername + " 341 " + nick + " #" + channel + " " + user + "\n"
#define RPL_INVITELIST 346
#define RPL_ENDOFINVITELIST 347
#define RPL_EXCEPTLIST 348
#define RPL_ENDOFEXCEPTLIST 349
#define RPL_VERSION 351
#define RPL_NAMREPLY(servername, nick, symbol, channel, users) \
	":" + servername + " 353 " + nick + " " + symbol + " #" + channel + " :" + users + "\n"
#define RPL_ENDOFNAMES(servername, nick, channel) \
	":" + servername + " 366 " + nick + " #" + channel + " :End of /NAMES list\n"
#define RPL_BANLIST 367
#define RPL_ENDOFBANLIST 368
#define RPL_ENDOFWHOWAS 369
#define RPL_INFO 371
#define RPL_ENDOFINFO 374
#define RPL_MOTDSTART(servername, nick) \
	":" + servername + " 375 " + nick + " :- " + servername + " Message of the day - \n"
#define RPL_MOTD(servername, nick, motd) \
	":" + servername + " 372 " + nick + " :- " + motd + "\n"
#define RPL_ENDOFMOTD(servername, nick) \
	":" + servername + " 376 " + nick + " :End of /MOTD command\n"
#define RPL_WHOISHOST 378
#define RPL_WHOISMODES 379
#define RPL_YOUREOPER 381
#define RPL_REHASHING 382
#define RPL_TIME 391
#define ERR_UNKNOWNERROR 400
#define ERR_NOSUCHNICK(servername, nick) \
	":" + servername + " 401 " + nick + " :No such nick/channel\n"
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL(servername, nick, channel) \
	":" + servername + " 403 " + nick + " #" + channel + " :No such channel\n"
#define ERR_CANNOTSENDTOCHAN(servername, nick, channel) \
	":" + servername + " 404 " + nick + " #" + channel + " :Cannot send to channel\n"
#define ERR_TOOMANYCHANNELS(servername, nick, channel) \
	":" + servername + " 405 " + nick + " #" + channel + " :You have joined too many channels\n"
#define ERR_WASNOSUCHNICK 406
#define ERR_NOORIGIN 409
#define ERR_INPUTTOOLONG 417
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NOMOTD 422
#define ERR_NONICKNAMEGIVEN(servername) \
	":" + servername + " 431 " + servername + " :No nickname given\n"
#define ERR_ERRONEUSNICKNAME(servername, nick) \
	":" + servername + " 432 * " + nick + " :Erroneous nickname\n"
#define ERR_NICKNAMEINUSE(servername, nick) \
	":" + servername + " 433 * " + nick + " :Nickname is already in use\n"
#define ERR_USERNOTINCHANNEL(servername, nick, channel) \
	":" + servername + " 441 * " + nick + " #" + channel + " :They aren't on that channel\n"
#define ERR_NOTONCHANNEL(servername, nick, channel) \
	":" + servername + " 442 * " + nick + " #" + channel + " :You're not on that channel\n"
#define ERR_USERONCHANNEL(servername, nick, username, channel) \
	":" + servername + " 443 * " + nick + " " + username + " #" + channel + " :is already on channel\n"
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS(servername, nick, command) \
	":" + servername + " 461 " + nick + " " + command + " :Wrong num parameters\n"
#define ERR_ALREADYREGISTRED(servername, nick) \
	":" + servername + " 462 " + nick + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(servername, nick) \
	":" + servername + " 464 " + nick + " :Password incorrect\n"
#define ERR_YOUREBANNEDCREEP(servername, nick) \
	":" + servername + " 465 " + nick + " :You are banned from this server\n"
#define ERR_KEYSET(servername, nick, channel) \
	":" + servername + " 467 " + nick + " #" + channel + " :Channel key already set\n"
#define ERR_CHANNELISFULL(servername, nick, channel) \
	":" + servername + " 471 " + nick + " #" + channel + " :Cannot join channel , channel is full(+l)\n"
#define ERR_UNKNOWNMODE(servername, nick, mode) \
	":" + servername + " 472 " + nick + " " + mode + " :is unknown mode char to me for that channel\n"
#define ERR_INVITEONLYCHAN(servername, nick, channel) \
	":" + servername + " 473 " + nick + " #" + channel + " :Cannot join channel (+i)\n"
#define ERR_BANNEDFROMCHAN(servername, nick, channel) \
	":" + servername + " 474 " + nick + " #" + channel + " :Cannot join channel (+b)\n"
#define ERR_BADCHANNELKEY(servername, nick, channel) \
	":" + servername + " 475 " + nick + " #" + channel + " :Bad channel key\n"
#define ERR_BADCHANMASK(servername, nick) \
	":" + servername + " 476 * " + nick + " :Bad Channel Mask\n"
#define ERR_NOPRIVILEGES 481
#define ERR_CHANOPRIVSNEEDED(servername, nick, channel) \
	":" + servername + " 482 " + nick + " #" + channel + " :You're not channel operator\n"
#define ERR_CANTKILLSERVER 483
#define ERR_NOOPERHOST 491
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH(servername, nick) \
	":" + servername + " 502 " + nick + " :Cant change mode for other users\n"
#define ERR_HELPNOTFOUND 524
#define ERR_INVALIDKEY 525
#define RPL_STARTTLS 670
#define RPL_WHOISSECURE 671
#define ERR_STARTTLS 691
#define ERR_INVALIDMODEPARAM 696
#define RPL_HELPSTART 704
#define RPL_HELPTXT 705
#define RPL_ENDOFHELP 706
#define ERR_NOPRIVS 723
#define RPL_LOGGEDIN 900
#define RPL_LOGGEDOUT 901
#define ERR_NICKLOCKED 902
#define RPL_SASLSUCCESS 903
#define ERR_SASLFAIL 904
#define ERR_SASLTOOLONG 905
#define ERR_SASLABORTED 906
#define ERR_SASLALREADY 907
#define RPL_SASLMECHS 908

// FICTICIAS
#define ERR_WRONGPASS(servername, nick) \
	":" + servername + "  " + nick + " :Wrong pass\n"
#define ERR_NOTPASSNEEDED(servername, nick) \
	":" + servername + "  " + nick + " :This server does not need any pass\n"

#define RPL_CUSTOM_JOIN(user, channel) ":" + user + " JOIN #" + channel + "\n"
#define RPL_CUSTOM_KICK(user, channel, nick, reason) \
	":" + user + " KICK #" + channel + " " + nick + " :" + reason + "\n"
#define RPL_CUSTOM_MODE(user, channel, mode) \
	":" + user + " MODE #" + channel + " " + mode + "\n"
#define RPL_CUSTOM_INVITE(user, channel, nick) \
	":" + user + " INVITE " + nick + " #" + channel + "\n"
#endif
