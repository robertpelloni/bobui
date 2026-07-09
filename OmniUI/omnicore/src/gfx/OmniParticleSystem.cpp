#include "OmniParticleSystem.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>

OmniParticleSystem::OmniParticleSystem(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_isRunning(false),
      m_emissionRate(10),
      m_gravity(9.8),
      m_particleColor(QColor("#00F0FF")), // Cyberpunk Cyan
      m_maxParticles(5000)
{
    setObjectName("OmniParticleSystem");
    setFlag(ItemHasContents, true);
    
    // Tie the particle physics natively to the DSP audio clock
}

OmniParticleSystem::~OmniParticleSystem() = default;

bool OmniParticleSystem::isRunning() const { return m_isRunning; }
void OmniParticleSystem::setIsRunning(bool running) {
    if (m_isRunning != running) {
        m_isRunning = running;
        emit isRunningChanged();
    }
}

int OmniParticleSystem::emissionRate() const { return m_emissionRate; }
void OmniParticleSystem::setEmissionRate(int rate) {
    if (m_emissionRate != rate) {
        m_emissionRate = rate;
        emit emissionRateChanged();
    }
}

qreal OmniParticleSystem::gravity() const { return m_gravity; }
void OmniParticleSystem::setGravity(qreal g) {
    if (!qFuzzyCompare(m_gravity, g)) {
        m_gravity = g;
        emit gravityChanged();
    }
}

QColor OmniParticleSystem::particleColor() const { return m_particleColor; }
void OmniParticleSystem::setParticleColor(const QColor& color) {
    if (m_particleColor != color) {
        m_particleColor = color;
        emit particleColorChanged();
    }
}

void OmniParticleSystem::burst(int count, const QPointF& pos) {
    for (int i = 0; i < count; ++i) {
        spawnParticle(pos);
    }
    update();
}

void OmniParticleSystem::spawnParticle(const QPointF& pos) {
    if (m_particles.size() >= m_maxParticles) return;

    OmniParticle p;
    p.position = pos;
    
    // Random radial velocity
    qreal angle = QRandomGenerator::global()->generateDouble() * 2.0 * 3.14159265;
    qreal speed = QRandomGenerator::global()->generateDouble() * 5.0 + 1.0;
    p.velocity = QPointF(std::cos(angle) * speed, std::sin(angle) * speed);
    
    p.maxLife = QRandomGenerator::global()->generateDouble() * 2.0 + 0.5; // 0.5s to 2.5s
    p.life = p.maxLife;
    
    // Slight color variation
    int h, s, l;
    m_particleColor.getHsl(&h, &s, &l);
    h = (h + QRandomGenerator::global()->bounded(-10, 10)) % 360;
    p.color = QColor::fromHsl(h, s, l);

    m_particles.append(p);
}

void OmniParticleSystem::updatePhysics(qreal dt) {
    for (int i = m_particles.size() - 1; i >= 0; --i) {
        OmniParticle& p = m_particles[i];
        
        p.life -= dt;
        if (p.life <= 0) {
            m_particles.removeAt(i);
            continue;
        }

        p.velocity.setY(p.velocity.y() + (m_gravity * dt));
        p.position += p.velocity;
    }
}

void OmniParticleSystem::onClockTick(int stepNumber) {
    Q_UNUSED(stepNumber);
    if (!m_isRunning) return;

    // Simulation delta time (Assuming 60fps equivalent tick rate from MasterClock)
    qreal dt = 0.016; 
    
    updatePhysics(dt);

    if (m_particles.size() < m_maxParticles) {
        for (int i = 0; i < m_emissionRate; ++i) {
            spawnParticle(QPointF(width() / 2.0, height() / 2.0)); // Emit from center
        }
    }

    update(); // Trigger paint
}

void OmniParticleSystem::paint(QPainter *painter) {
    if (m_particles.isEmpty()) return;

    painter->setRenderHint(QPainter::Antialiasing, false); // Turn off AA for massive batch performance
    
    // Draw all particles in a highly optimized loop
    for (const auto& p : std::as_const(m_particles)) {
        qreal alpha = p.life / p.maxLife;
        QColor c = p.color;
        c.setAlphaF(alpha);
        
        painter->setPen(c);
        painter->drawPoint(p.position);
        
        // Add a slight glow effect if life is high
        if (alpha > 0.5) {
            c.setAlphaF(alpha * 0.3);
            painter->setBrush(c);
            painter->setPen(BobUI::NoPen);
            painter->drawEllipse(p.position, 2, 2);
        }
    }
}
