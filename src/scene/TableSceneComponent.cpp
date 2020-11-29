#include "TableSceneComponent.hpp"

TableSceneComponent::TableSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, int rowLimit, const std::vector<int>& columnCharacterLimits) : SceneComponent(relativePosition, relativeSize, componentClass), rowLimit_(rowLimit), columnCharacterLimits_(columnCharacterLimits)
{
    // Set text
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(100);
    rectangleShape_.setFillColor(backgroundColor);
    // Characters per row
    rowCharacterLimit_ = 0;
    for(auto characterLimit : columnCharacterLimits_)
    {
        rowCharacterLimit_ += characterLimit;
    }
    // Character needed for sizing
    character_.setString(" ");
    character_.setFont(font);
    character_.setCharacterSize(text_.getCharacterSize());
    // Set size and position
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y});
    // Set size of deque
    table_.resize(rowLimit_);
}

void TableSceneComponent::HandlePacket(sf::Packet packet)
{
    NetworkMessageType messageType;
    packet >> messageType;
    if(componentClass_ == "chat")
    {
        if(messageType == CHAT_MESSAGE)
        {
            std::string playerName;
            std::string message;
            packet >> playerName >> message;
            AddRow({playerName, message});
        }
        else if (messageType == PING)
        {
            std::string ping;
            packet >> ping;
            AddRow({"PING", ping});
        }
    }
    else if (componentClass_ == "playerlist")
    {
        if(messageType == CLIENT_CONNECT)
        {
            std::string clientName;
            sf::Int32 id = -1;
            packet >> clientName >> id;
            ReplaceIndex(id, {std::to_string(id), clientName});
        }
        else if(messageType == CLIENT_DISCONNECT)
        {
            std::string clientName;
            sf::Int32 id = -1;
            packet >> clientName >> id;
            ReplaceIndex(id, {"",""});
        }
    }
    else if (componentClass_ == "scorelist")
    {
        if(messageType == CLIENT_RANK)
        {
            sf::Int32 id = -1;
            std::string clientName;
            sf::Int32 ranking = -1;
            packet >> id >> clientName >> ranking;
            ReplaceIndex(ranking, {std::to_string(ranking), clientName, std::to_string(id)});
        }
    }
}

void TableSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    // switch(event.type)
    // {
    //     case sf::Event::MouseMoved:
    //         // if(IsMouseHovering(window)) // TODO SCROLL BAR
    //         // {
    //         //     SetBackgroundColor(highlightColor_);
    //         // }
    //         // else
    //         // {
    //         //     SetBackgroundColor(backgroundColor_);
    //         // }
    //         break;
    //     case sf::Event::MouseButtonPressed:
    //         // if(IsMouseHovering(window)) // TODO SCROLL BAR
    //         // {
    //         //     onClick_();
    //         // }
    //         break;
    //     default:
    //         break;
    // }
}

void TableSceneComponent::Update(const sf::Time& deltaTime)
{
}

void TableSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(rectangleShape_);
    window.draw(text_);
}

void TableSceneComponent::Init()
{
    if(componentClass_ == "scorelist")
    {
        ReplaceIndex(0, {"POS", "PLAYER","ID"});
    }
}

void TableSceneComponent::Reset()
{
    table_.clear();
    table_.resize(rowLimit_);
    text_.setString("");
}

int TableSceneComponent::AddRow(const std::vector<std::string>& row)
{
    if(table_.size() == rowLimit_)
    {
        table_.pop_front();
    }
    table_.push_back(row);
    UpdateText();
    return table_.size() - 1;
}

void TableSceneComponent::RemoveIndex(int i)
{
    table_.erase(table_.begin() + i);
    UpdateText();
}

void TableSceneComponent::ReplaceIndex(int i, const std::vector<std::string>& row)
{
    table_[i] = row;
    UpdateText();
}

void TableSceneComponent::SetPosition(const sf::Vector2f& position)
{
    rectangleShape_.setPosition(position);
    sf::Vector2f textPosition(
        position.x + rectangleShape_.getGlobalBounds().width / 2.0f - rowCharacterLimit_ * character_.getGlobalBounds().width / 2.0f,
        position.y + rectangleShape_.getGlobalBounds().height / 2.0f - rowLimit_ * (character_.getGlobalBounds().height + character_.getGlobalBounds().top) / 2.0f 
    );
    text_.setPosition(textPosition);
}

void TableSceneComponent::SetSize(const sf::Vector2f& size)
{
    rectangleShape_.setSize(size);

    float desiredScale = 0.95f / (rowLimit_ * (character_.getLocalBounds().height + character_.getLocalBounds().top) / size.y);
    text_.setScale(desiredScale, desiredScale);
    character_.setScale(desiredScale, desiredScale);
    if(rowCharacterLimit_ * character_.getGlobalBounds().width > size.x * 0.9)
    {
        desiredScale = 0.95 / (rowCharacterLimit_ * character_.getLocalBounds().width / size.x);
        text_.setScale(desiredScale, desiredScale);
        character_.setScale(desiredScale, desiredScale);
    }
}

bool TableSceneComponent::IsMouseHovering(sf::RenderWindow &window)
{
    // float mouseX = sf::Mouse::getPosition(window).x;
    // float mouseY = sf::Mouse::getPosition(window).y; TODO SCROLLBAR
    // float minX = rectangleShape_.getPosition().x;
    // float minY = rectangleShape_.getPosition().y;
    // float maxX = minX + rectangleShape_.getLocalBounds().width;
    // float maxY = minY + rectangleShape_.getLocalBounds().height;
    // if(minX < mouseX && mouseX < maxX && minY < mouseY && mouseY < maxY)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }
    return false;
}

void TableSceneComponent::UpdateText()
{
    std::stringstream ss;
    for(auto row : table_)
    {
        for(int i = 0; i < row.size(); i++)
        {
            if(row[i].size() == columnCharacterLimits_[i])
            {
                ss << row[i];
            }
            else if (row[i].size() > columnCharacterLimits_[i])
            {
                ss << row[i].substr(0,columnCharacterLimits_[i]);
            }
            else
            {
                ss << row[i].append(columnCharacterLimits_[i] - row[i].size(), ' ');
            }
        }
        ss << std::endl;
    }
    text_.setString(ss.str());
}