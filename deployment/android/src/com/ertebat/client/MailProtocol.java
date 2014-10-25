package com.ertebat.client;

import java.security.InvalidParameterException;

public enum MailProtocol {
	MP_IMAPS("imaps"),
	MP_POP3S("pop3s");
	
	private MailProtocol(String title) {
		this.title = title;
	}
	
	private String title;
	
	@Override
	public String toString() {
		return title;
	}
	
	public static MailProtocol getByOrdinal(int index) {
		for (MailProtocol protocol : MailProtocol.values()) {
			if (protocol.ordinal() == index)
				return protocol;
		}
		
		throw new InvalidParameterException();
	}
}
