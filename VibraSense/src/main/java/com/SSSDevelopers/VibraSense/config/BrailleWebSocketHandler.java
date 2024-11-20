package com.SSSDevelopers.VibraSense.config;

import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;

public class BrailleWebSocketHandler extends TextWebSocketHandler {
    private static final Logger logger = LoggerFactory.getLogger(BrailleWebSocketHandler.class);

    // Store connected sessions
    private static final ConcurrentHashMap<String, WebSocketSession> sessions = new ConcurrentHashMap<>();

    // Store ESP32 session separately
    private static WebSocketSession esp32Session = null;

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        logger.info("New WebSocket connection established: " + session.getId());
        sessions.put(session.getId(), session);

        // Check if this is the ESP32 connection (you might want to add some authentication here)
        String userAgent = session.getHandshakeHeaders().getFirst("User-Agent");
        if (userAgent != null && userAgent.contains("ESP32")) {
            esp32Session = session;
            logger.info("ESP32 connected with session ID: " + session.getId());
        }
    }

    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws IOException {
        String payload = message.getPayload();
        logger.info("Received message: " + payload);

        try {
            // If message is from web client, forward to ESP32
            if (esp32Session != null && esp32Session.isOpen() && !session.equals(esp32Session)) {
                esp32Session.sendMessage(message);
                logger.info("Forwarded message to ESP32");
            }
            // If message is from ESP32, forward to all web clients
            else if (session.equals(esp32Session)) {
                for (WebSocketSession webClient : sessions.values()) {
                    if (!webClient.equals(esp32Session) && webClient.isOpen()) {
                        webClient.sendMessage(message);
                    }
                }
            }
        } catch (IOException e) {
            logger.error("Error sending message: " + e.getMessage());
        }
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) {
        logger.info("WebSocket connection closed: " + session.getId());
        sessions.remove(session.getId());

        if (session.equals(esp32Session)) {
            esp32Session = null;
            logger.info("ESP32 disconnected");
        }
    }

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception) {
        logger.error("WebSocket transport error: " + exception.getMessage());
        sessions.remove(session.getId());

        if (session.equals(esp32Session)) {
            esp32Session = null;
            logger.error("ESP32 connection error");
        }
    }
}