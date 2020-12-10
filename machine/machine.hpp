//
//  machine.hpp
//  Machine
//
//  Created by Daniel Pav on 4/27/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef machine_hpp
#define machine_hpp

namespace machine {
template<typename TInput, typename TToken, typename TOutput>
class Machine
{
public:
    using Action = void (*)(TOutput&, const TToken&);
    using Filter = bool (*)(const TToken&);
    using Advance = void (*)(TInput&, TToken&);
    
public:
    static Filter noFilter() { return nullptr; }
    static Action doNothing() { return nullptr; }

public:
    class Link;
    
public:    
    class State
    {
        friend class Link;
        friend class Machine;
        
    public:
        State(Action action = doNothing()) :
        m_action(action),
        m_firstLink(nullptr)
        { }
        ~State() = default;
        
    private:
        State(const State&) = delete;
        State& operator=(const State&) = delete;
        State(State&&) = delete;
        State& operator=(State&&) = delete;

    private:
        void appendLink(Link& link);
        const State * nextState(const TToken& token, TOutput& output) const;

    private:
        Action m_action;
        Link * m_firstLink;
    };

public:
    class Link
    {
    public:
        Link(
            State& from,
            const State& to,
            Filter filter = noFilter(),
            Action action = doNothing()) :
        m_nextLink(nullptr),
        m_to(to),
        m_filter(filter),
        m_action(action)
        {
            from.appendLink(*this);
        }

    public:
        static void appendToChain(Link& head, Link& node);
        static const State * advanceState(const Link * head, TOutput& output, const TToken& token);
        
    private:
        Link * m_nextLink;
        const State& m_to;
        Filter m_filter;
        Action m_action;
    };
    
public:
    Machine(
        const State& start,
        const State& stop,
        Advance advance) :
    m_start(start),
    m_stop(stop),
    m_advance(advance)
    { }

public:
    bool process(TInput& input, TOutput& output) const;
    
private:
    const State& m_start;
    const State& m_stop;
    Advance m_advance;
};


template<
typename TInput,
typename TToken,
typename TOutput>
void Machine<TInput, TToken, TOutput>::State::appendLink(
                                                         Link& link)
{
    if (m_firstLink)
        Link::appendToChain(*m_firstLink, link);
    else
        m_firstLink = &link;
}

template<
typename TInput,
typename TToken,
typename TOutput>
const typename Machine<TInput, TToken, TOutput>::State *
Machine<TInput, TToken, TOutput>::State::nextState(
                                                   const TToken& token,
                                                   TOutput& output) const
{
    const State * next = Link::advanceState(m_firstLink, output, token);
    if (next && next->m_action)
        (*next->m_action)(output, token);
    return next;
}

template<typename TInput, typename TToken, typename TOutput>
void Machine<TInput, TToken, TOutput>::Link::appendToChain(Link& head, Link& node)
{
    Link * l = &head;
    while (l->m_nextLink)
    {
        l = l->m_nextLink;
    }
    l->m_nextLink = &node;
}

template<typename TInput, typename TToken, typename TOutput>
const typename Machine<TInput, TToken, TOutput>::State *
Machine<TInput, TToken, TOutput>::Link::advanceState(
                                                     const Machine<TInput, TToken, TOutput>::Link * head,
                                                     TOutput& output,
                                                     const TToken& token)
{
    const Link * l = head;
    while (l)
    {
        if (!l->m_filter || (*(l->m_filter))(token))
        {
            if (l->m_action)
                (*l->m_action)(output, token);
            return &l->m_to;
        }
        l = l->m_nextLink;
    }
    return nullptr;
}

template<typename TInput, typename TToken, typename TOutput>
bool Machine<TInput, TToken, TOutput>::process(TInput& input, TOutput& output) const
{
    const State * state = &m_start;
    TToken token;
    while (state != &m_stop)
    {
        (*m_advance)(input, token);
        state = state->nextState(token, output);
        if (!state)
            return false;
    }
    return true;
}
}//namespace machine

#endif /* machine_hpp */
