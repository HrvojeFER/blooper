#ifndef BLOOPER_PROJECT_WINDOW_HPP
#define BLOOPER_PROJECT_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectWindow : public WindowBase
{
 public:
  explicit ProjectWindow(AbstractContext& context);

 private:
};


void showProject(AbstractContext& context);

BLOOPER_NAMESPACE_END

#endif //BLOOPER_PROJECT_WINDOW_HPP
